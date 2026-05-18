#include <graphics.h>  // 引入图形库，用于图形显示
#include "initset.h"    // 引入初始化设置头文件
#include "bits/stdc++.h" // 引入标准库头文件，包含常用库
#pragma comment(lib,"Winmm.lib")  // 引入Winmm库，用于音频控制

// 定义块（block）结构体
extern struct block {
public:
    int x;  // 块的x坐标
    int y;  // 块的y坐标
    block(int x, int y) {  // 构造函数，初始化块的坐标
        this->x = x;
        this->y = y;
    }
};

// 外部声明的变量
extern int map[BLOCK_WIDTH + 10][BLOCK_HEIGHT + 10];  // 地图数组
extern std::vector<block*> openlist;  // openlist，用于存储待扩展的块
extern std::vector<block*> list;  // list，用于存储已访问的块
std::vector<block*> lists;  // 临时存储块
extern block* myprev[BLOCK_WIDTH + 10][BLOCK_HEIGHT + 10];  // 记录每个块的前一个块，用于回溯路径

// 函数声明
void dfsmap();  // 深度优先搜索地图生成函数
void dfsthrough(int x, int y);  // 深度优先搜索的过程函数
bool check(int x, int y);  // 检查当前块是否可以扩展
void blockColor(int x, int y, COLORREF RGB);  // 设置块的颜色
void dfsinit(void);  // 初始化深度优先搜索
void primAddList(int x, int y);  // 生成Prim算法时的辅助函数
void reDarw(void);  // 重绘函数
void prim(void);  // Prim算法生成路径
bool primThrough(int x, int y);  // Prim算法的路径扩展函数

// 深度优先搜索初始化函数
void dfsinit() {
    openlist.clear();  // 清空openlist
    for (int y = 0; y <= BLOCK_HEIGHT; y++) {  // 遍历整个地图并初始化为障碍
        for (int x = 0; x <= BLOCK_WIDTH; x++) {
            map[x][y] = 1;  // 设置为障碍
            myprev[x][y] = nullptr;  // 初始化前驱块为空
        }
    }
    map[1][1] = 0;  // 设置起点为可通行
    block* temp = new block(1, 1);  // 创建起点块
    openlist.push_back(temp);  // 将起点加入openlist
    reDarw();  // 重绘地图
    dfsmap();  // 执行深度优先搜索生成地图
}

// 深度优先搜索生成地图
void dfsmap() {
    mciSendString(_T("open orb.mp3 alias bkmusic1"), NULL, 0, NULL);  // 播放背景音乐
    mciSendString(_T("play bkmusic1 repeat"), NULL, 0, NULL);  // 设置背景音乐循环播放

    while (openlist.size() > 0) {  // 当openlist中还有块待处理
        int Index = (int)openlist.size() - 1;  // 获取openlist的最后一个元素的索引
        block* A = openlist[Index];  // 获取当前块
        while (!check(A->x, A->y)) {  // 如果当前块没有可扩展的相邻块
            A = openlist[Index];  // 重新获取当前块
            if (Index == 0) return;  // 如果已经没有可扩展的块，退出
            --Index;  // 否则继续向前查找
        }
        dfsthrough(A->x, A->y);  // 处理当前块
    }

    reDarw();  // 重绘地图
    mciSendString(_T("close bkmusic1"), NULL, 0, NULL);  // 停止播放背景音乐
}

// 执行深度优先搜索的扩展步骤
void dfsthrough(int x, int y) {
    list.clear();  // 清空list
    lists.clear();  // 清空lists
    for (int i = 0; i < 4; ++i) {  // 检查四个方向的相邻块
        // 检查是否越界，并且是否为障碍
        if (y + dir2[i][1] > 0 && x + dir2[i][0] > 0 &&
            y + dir2[i][1] < BLOCK_HEIGHT && x + dir2[i][0] < BLOCK_WIDTH &&
            map[x + dir2[i][0]][y + dir2[i][1]] == 1) {
            block* temp = new block(x + dir1[i][0], y + dir1[i][1]);  // 创建新的块
            block* a = new block(x + dir1[i][0], y + dir1[i][1]);  // 新块
            block* aa = new block(x + dir2[i][0], y + dir2[i][1]);  // 相邻的块
            list.push_back(a);  // 将新块加入list
            lists.push_back(aa);  // 将相邻块加入lists
        }
    }

    if (list.size() != 0) {  // 如果有可扩展的块
        int Index = rand() % list.size();  // 随机选择一个可扩展块
        block* B = list[Index];  // 获取该块
        block* BB = lists[Index];  // 获取相邻块
        map[B->x][B->y] = 0;  // 设置块为可通行
        map[BB->x][BB->y] = 0;  // 设置相邻块为可通行
        blockColor(B->x, B->y, road);  // 给块上色
        blockColor(BB->x, BB->y, road);  // 给相邻块上色
        FlushBatchDraw();  // 刷新图形显示
        openlist.push_back(BB);  // 将相邻块加入openlist
    }
}

// 检查当前块是否可以扩展（是否存在未访问的相邻块）
bool check(int x, int y) {
    bool temp = 0;  // 用于标记是否可以扩展
    for (int i = 0; i < 4; ++i) {  // 检查四个方向
        // 检查是否越界，并且是否为障碍
        if (y + dir2[i][1] > 0 && x + dir2[i][0] > 0 &&
            y + dir2[i][1] < BLOCK_HEIGHT && x + dir2[i][0] < BLOCK_WIDTH &&
            map[x + dir2[i][0]][y + dir2[i][1]] == 1) {
            temp = 1;  // 如果存在未访问的相邻块，标记为可以扩展
        }
    }
    return temp;  // 返回是否可以扩展
}
