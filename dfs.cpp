#include <graphics.h>  // 引入图形库，用于图形显示
#include "initset.h"    // 引入初始化设置头文件
#include "bits/stdc++.h" // 引入标准库头文件，包含常用库
#pragma comment(lib,"Winmm.lib") // 引入Winmm库，用于音频控制

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
extern int map[BLOCK_WIDTH + 10][BLOCK_HEIGHT + 10]; // 地图数组
extern block* myprev[BLOCK_WIDTH + 10][BLOCK_HEIGHT + 10];  // 记录每个块的前一个块，用于回溯路径

// 函数声明
void blockColor(int x, int y, COLORREF RGB); // 设置块颜色的函数
void dfs(void);  // 深度优先搜索（DFS）算法
void drawShortestPath(block* start, block* end); // 绘制最短路径的函数

// 深度优先搜索（DFS）算法实现
void dfs(void) {
    std::stack<block*> dfsStack;  // 创建一个栈用于DFS
    int cnt = 1920;  // 初始化计数器，用于颜色变化
    COLORREF dfsPath = RGB(190, 190, cnt / 10);  // 初始化路径颜色
    block* start = new block(1, 1);  // 起点为(1, 1)
    block* end = new block(BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1);  // 终点为地图的右下角
    dfsStack.push(start);  // 将起点加入栈
    mciSendString(_T("open portal.mp3 alias bkmusic2"), NULL, 0, NULL);  // 播放背景音乐
    mciSendString(_T("play bkmusic2 repeat"), NULL, 0, NULL);  // 设置背景音乐循环播放

    while (!dfsStack.empty()) {  // 当栈不为空时继续搜索
        block* current = dfsStack.top();  // 获取栈顶的块
        dfsStack.pop();  // 弹出栈顶块
        if (current->x == end->x && current->y == end->y) {  // 如果当前块是终点
            drawShortestPath(start, end);  // 绘制最短路径
            mciSendString(_T("close bkmusic2"), NULL, 0, NULL);  // 停止背景音乐
            blockColor(1, 1, beginPoint);  // 给起点上色
            blockColor(BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1, endPoint);  // 给终点上色
            FlushBatchDraw();  // 刷新显示
            return;  // 结束搜索
        }
        for (int i = 0; i < 4; ++i) {  // 对当前块的四个邻居进行搜索
            // 判断邻居是否在地图范围内且是可通行的
            if (current->x + dir1[i][0] > 0 && current->y + dir1[i][1] > 0 &&
                current->x + dir1[i][0] < BLOCK_WIDTH && current->y + dir1[i][1] < BLOCK_HEIGHT &&
                map[current->x + dir1[i][0]][current->y + dir1[i][1]] == 0 &&
                myprev[current->x + dir1[i][0]][current->y + dir1[i][1]] == nullptr) {
                block* temp = new block(current->x + dir1[i][0], current->y + dir1[i][1]);  // 创建新块
                myprev[temp->x][temp->y] = current;  // 记录前驱块
                dfsStack.push(temp);  // 将新块加入栈
                cnt++;  // 计数器增加
                dfsPath = RGB(max(50, 190 - (cnt / 50)), max(50, 190 - (cnt / 50)), min(255, cnt / 10));  // 更新路径颜色
                blockColor(temp->x, temp->y, dfsPath);  // 给新块上色
                FlushBatchDraw();  // 刷新显示
            }
        }
    }
    exit(1);  // 如果栈为空且没有找到路径，退出程序
}
