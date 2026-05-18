#include <graphics.h>               // 图形库头文件，用于绘图操作
#include "initset.h"                // 初始化设置头文件
#include "bits/stdc++.h"           // 常用C++标准库的头文件集合
#pragma comment(lib,"Winmm.lib")  // 链接Winmm库，用于音频控制

// 声明一个结构体block，表示一个网格的坐标
extern struct block {
public:
    int x;  // x坐标
    int y;  // y坐标
    block(int x, int y) {  // 构造函数，初始化x和y
        this->x = x;
        this->y = y;
    }
};

// 声明一个二维数组map表示网格地图，openlist表示待处理的块，list是临时存储块的容器，myprev用于存储每个位置的前驱
extern int map[BLOCK_WIDTH + 10][BLOCK_HEIGHT + 10];
extern std::vector<block*> openlist;
extern std::vector<block*> list;
extern block* myprev[BLOCK_WIDTH + 10][BLOCK_HEIGHT + 10];

// 函数声明：这些函数实现了图形绘制和Kruskal算法相关操作
void blockColor(int x, int y, COLORREF RGB);
void kruskalinit(void);
void kruskalAddList(int x, int y);
bool kruskalThrough(int x, int y);
void reDarw(void);
void kruskal(void);
int find(int x);
void unite(int x, int y);
bool isSameSet(int x, int y);

// 用于存储并查集的父节点
std::vector<int> parent;

// 初始化Kruskal算法相关的数据
void kruskalinit(void) {
    openlist.clear();  // 清空待处理列表
    for (int y = 0; y <= BLOCK_HEIGHT; y++) {  // 初始化map和myprev数组
        for (int x = 0; x <= BLOCK_WIDTH; x++) {
            map[x][y] = 1;  // 设置每个块为墙壁
            myprev[x][y] = nullptr;  // 设置每个位置的前驱为null
        }
    }
    map[1][1] = 0;  // 设置起点为开放区域
    map[BLOCK_WIDTH - 1][BLOCK_HEIGHT - 1] = 0;  // 设置终点为开放区域

    // 初始化并查集父节点
    parent.resize((BLOCK_WIDTH + 1) * (BLOCK_HEIGHT + 1));
    for (int i = 0; i < parent.size(); i++) {
        parent[i] = i;  // 每个节点自成一组
    }
    reDarw();  // 重新绘制图形
    kruskalAddList(1, 1);  // 将起点加入待处理列表
    kruskal();  // 执行Kruskal算法
}

// 获取网格位置(x, y)在并查集中的索引
int getIndex(int x, int y) {
    return (y - 1) * (BLOCK_WIDTH - 1) + x;
}

// 判断从当前网格是否可以继续扩展到相邻的开放区域
bool kruskalThrough(int x, int y) {
    list.clear();  // 清空临时存储的块列表
    for (int i = 0; i < 4; ++i) {  // 尝试四个方向
        // 判断是否越界，并且相邻位置是否是开放区域
        if (y + dir2[i][1] > 0 && x + dir2[i][0] > 0 &&
            y + dir2[i][1] < BLOCK_HEIGHT && x + dir2[i][0] < BLOCK_WIDTH &&
            map[x + dir2[i][0]][y + dir2[i][1]] == 0) {
            block* temp = new block(x + dir1[i][0], y + dir1[i][1]);
            list.push_back(temp);  // 将符合条件的块加入临时列表
        }
    }
    // 如果有符合条件的块，随机选择一个并更新状态
    if (list.size() != 0) {
        int Index = rand() % list.size();  // 随机选择一个块
        block* A = list[Index];
        map[A->x][A->y] = 0;  // 更新地图，标记为开放区域
        unite(getIndex(x, y), getIndex(A->x, A->y));  // 合并并查集
        blockColor(A->x, A->y, road);  // 绘制块的颜色
        FlushBatchDraw();  // 刷新显示
        return true;
    }
    else {
        return false;  // 如果没有符合条件的块，返回false
    }
}

// 执行Kruskal算法，生成迷宫
void kruskal(void) {
    mciSendString(_T("open orb.mp3 alias bkmusic1"), NULL, 0, NULL);  // 播放背景音乐
    mciSendString(_T("play bkmusic1 repeat"), NULL, 0, NULL);  // 循环播放音乐

    // 不断从openlist中取出一个块并扩展，直到openlist为空
    while (openlist.size() > 0) {
        int Index = rand() % openlist.size();  // 随机选择一个块
        block* begin = openlist[Index];
        kruskalAddList(begin->x, begin->y);  // 扩展相邻块
        kruskalThrough(begin->x, begin->y);  // 执行扩展
        map[begin->x][begin->y] = 0;  // 标记块为开放区域
        blockColor(begin->x, begin->y, road);  // 绘制块的颜色
        FlushBatchDraw();  // 刷新显示
        openlist.erase(openlist.begin() + Index);  // 从待处理列表中移除该块
    }
    // 最后尝试从终点附近扩展
    for (int i = 0; i < 4; ++i) {
        kruskalThrough(BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1);
        kruskalThrough(BLOCK_WIDTH - 3, BLOCK_HEIGHT - 3);
    }
    reDarw();  // 重新绘制图形
    mciSendString(_T("close bkmusic1"), NULL, 0, NULL);  // 关闭音乐
}

// 查找并查集的根节点
int find(int x) {
    if (x != parent[x]) {
        parent[x] = find(parent[x]);  // 路径压缩
    }
    return parent[x];
}

// 合并两个集合
void unite(int x, int y) {
    parent[find(x)] = find(y);  // 将两个集合合并
}

// 判断两个元素是否属于同一集合
bool isSameSet(int x, int y) {
    return find(x) == find(y);
}

// 将相邻的墙壁块加入待处理列表
void kruskalAddList(int x, int y) {
    for (int i = 0; i < 4; ++i) {  // 尝试四个方向
        if (x + dir2[i][0] > 0 && y + dir2[i][1] > 0 &&
            x + dir2[i][0] < BLOCK_WIDTH && y + dir2[i][1] < BLOCK_HEIGHT &&
            map[x + dir2[i][0]][y + dir2[i][1]] == 1 &&  // 如果是墙壁
            !isSameSet(getIndex(x, y), getIndex(x + dir2[i][0], y + dir2[i][1]))) {  // 且不在同一集合
            block* temp = new block(x + dir2[i][0], y + dir2[i][1]);
            openlist.push_back(temp);  // 加入待处理列表
            map[x + dir2[i][0]][y + dir2[i][1]] = 2;  // 标记为已处理
        }
    }
}
