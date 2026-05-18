#pragma once  // 预处理指令，确保该头文件只被编译一次

// 枚举类型 Info，用于定义常量
enum Info {
    WIDTH = 1920,      // 屏幕宽度
    HEIGHT = 1080,     // 屏幕高度
    BLOCK_XY = 12,     // 网格的行列数
};

// 计算每个网格的宽度和高度
const int BLOCK_WIDTH = WIDTH / (BLOCK_XY);   // 每个网格的宽度
const int BLOCK_HEIGHT = HEIGHT / (BLOCK_XY); // 每个网格的高度

// 方向数组 dir1，表示四个基本的上下左右方向
const int dir1[][2] = { {0,1},{0,-1},{1,0},{-1,0} };  // 上下左右的偏移量

// 方向数组 dir2，表示步长为2的上下左右方向
const int dir2[][2] = { {0,2},{0,-2},{2,0},{-2,0} };  // 上下左右的偏移量，步长为2

// 定义颜色宏
#define road COLORREF RGB(220,220,220)         // 道路的颜色，灰色
#define wal COLORREF RGB(30,30,30)             // 墙壁的颜色，深灰色
#define picked COLORREF RGB(150,30,30)         // 被选中区域的颜色，深红色
#define beginPoint COLORREF RGB(0,255,0)       // 起点的颜色，绿色
#define endPoint COLORREF RGB(255,0,0)         // 终点的颜色，红色
#define shortestPath COLORREF RGB(255,255,0)   // 最短路径的颜色，黄色
