#include <graphics.h>  // 引入图形库，用于图形显示
#include "initset.h"    // 引入初始化设置头文件
#include "bits/stdc++.h" // 引入标准库头文件，包含常用库
#include <random>  // 引入随机数生成库

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
extern std::vector<block*> openlist; // openlist，用于存储待扩展的块
extern std::vector<block*> list; // list，用于存储已访问的块
extern block* myprev[BLOCK_WIDTH + 10][BLOCK_HEIGHT + 10];  // 记录每个块的前一个块，用于回溯路径

// 函数声明
void blockColor(int x, int y, COLORREF RGB); // 设置块颜色的函数
void crossinit(void);  // 初始化函数
void crossDivide(int x1, int y1, int x2, int y2); // 递归分割函数
void reDarw(void); // 重绘函数

// 初始化地图并进行交叉分割
void crossinit(void) {
    openlist.clear();  // 清空openlist
    for (int y = 1; y <= BLOCK_HEIGHT; y++) {  // 遍历整个地图并初始化为0
        for (int x = 1; x <= BLOCK_WIDTH; x++) {
            map[x][y] = 0;  // 设置为可通行的空白块
            myprev[x][y] = nullptr;  // 初始化前驱块为空
        }
    }

    // 设置地图边界为不可通行的障碍块（值为1）
    for (int y = 0; y <= BLOCK_HEIGHT; y++) {
        map[0][y] = 1;
        map[BLOCK_WIDTH][y] = 1;
    }
    for (int x = 1; x <= BLOCK_WIDTH; x++) {
        map[x][0] = 1;
        map[x][BLOCK_HEIGHT] = 1;
    }

    reDarw();  // 重绘地图
    mciSendString(_T("open orb.mp3 alias bkmusic1"), NULL, 0, NULL);  // 播放背景音乐
    mciSendString(_T("play bkmusic1 repeat"), NULL, 0, NULL);  // 设置背景音乐循环播放

    crossDivide(0, 0, BLOCK_WIDTH, BLOCK_HEIGHT);  // 调用分割函数进行地图分割

    reDarw();  // 重绘地图
    mciSendString(_T("close bkmusic1"), NULL, 0, NULL);  // 停止背景音乐
}

// 生成指定范围的随机数
int randRange(int s, int e) {
    if (s == e) return e;  // 如果起始值和结束值相同，返回该值
    return rand() % (e - s + 1) + s;  // 否则返回范围内的随机数
}

// 递归地将区域分割成多个子区域，生成交叉结构
void crossDivide(int x1, int y1, int x2, int y2) {
    if (x2 - x1 <= 3 || y2 - y1 <= 3) return;  // 如果区域过小，不再继续分割

    // 随机选择垂直和水平方向的分割位置
    int vx = (randRange(x1 + 2, x2 - 2) | 1) - 1;  // 确保分割位置是奇数
    int hy = (randRange(y1 + 2, y2 - 2) | 1) - 1;  // 确保分割位置是奇数

    // 在水平方向上分割
    for (int x = x1 + 1; x <= x2 - 1; x++) {
        map[x][hy] = 1;  // 设置为墙壁
        blockColor(x, hy, wal);  // 给墙壁上色
    }
    FlushBatchDraw();  // 刷新图形显示

    // 在垂直方向上分割
    for (int y = y1 + 1; y <= y2 - 1; y++) {
        map[vx][y] = 1;  // 设置为墙壁
        blockColor(vx, y, wal);  // 给墙壁上色
    }
    FlushBatchDraw();  // 刷新图形显示

    // 随机生成打通的墙体的位置
    int xh1 = randRange(x1 + 1, vx - 1) | 1;  // 确保位置是奇数
    int xh2 = randRange(vx + 1, x2 - 1) | 1;  // 确保位置是奇数
    int yh1 = randRange(y1 + 1, hy - 1) | 1;  // 确保位置是奇数
    int yh2 = randRange(hy + 1, y2 - 1) | 1;  // 确保位置是奇数

    // 随机打通四个墙体中的三个
    std::vector<int> randDirs = { 0, 1, 2, 3 };  // 方向数组
    std::shuffle(randDirs.begin(), randDirs.end(), std::mt19937(std::random_device()()));  // 随机打乱方向数组

    for (int i = 0; i < 3; i++) {  // 打通三个墙体
        switch (randDirs[i]) {
        case 0:
            map[xh1][hy] = 0;  // 打通
            blockColor(xh1, hy, road);  // 给路径上色
            FlushBatchDraw();  // 刷新图形显示
            break;
        case 1:
            map[xh2][hy] = 0;  // 打通
            blockColor(xh2, hy, road);  // 给路径上色
            FlushBatchDraw();  // 刷新图形显示
            break;
        case 2:
            map[vx][yh1] = 0;  // 打通
            blockColor(vx, yh1, road);  // 给路径上色
            FlushBatchDraw();  // 刷新图形显示
            break;
        case 3:
            map[vx][yh2] = 0;  // 打通
            blockColor(vx, yh2, road);  // 给路径上色
            FlushBatchDraw();  // 刷新图形显示
            break;
        }
    }

    // 递归继续分割四个子区域
    crossDivide(x1, y1, vx, hy);
    crossDivide(vx, y1, x2, hy);
    crossDivide(x1, hy, vx, y2);
    crossDivide(vx, hy, x2, y2);
}
