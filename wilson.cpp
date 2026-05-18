#include <graphics.h>  // 引入图形库
#include "initset.h"    // 引入初始化设置头文件
#include "bits/stdc++.h" // 引入标准库头文件
#pragma comment(lib,"Winmm.lib")  // 链接Winmm.lib库，通常用于播放音频

// 定义块结构体，表示地图中的一个块
extern struct block {
public:
    int x;  // 块的x坐标
    int y;  // 块的y坐标
    block(int x, int y) {  // 构造函数，初始化块的坐标
        this->x = x;
        this->y = y;
    }
};

// 外部变量声明
extern int map[BLOCK_WIDTH + 10][BLOCK_HEIGHT + 10];  // 地图数组
extern block* myprev[BLOCK_WIDTH + 10][BLOCK_HEIGHT + 10];  // 记录每个块的前一个块
int vis[BLOCK_WIDTH + 10][BLOCK_HEIGHT + 10];  // 访问标记数组

// 函数声明
void blockColor(int x, int y, COLORREF RGB);  // 设置块的颜色
void wilsoninit(void);  // Wilson算法的初始化函数
void reDarw(void);  // 重绘函数
bool checkPath(int x, int y);  // 检查是否是有效路径
std::stack<std::pair<int, int>> next;  // 用于存储路径的栈

// Wilson算法的初始化函数
void wilsoninit(void) {
    // 初始化地图，所有块的值设为1（墙），访问标记设为1
    for (int y = 0; y <= BLOCK_HEIGHT; y++) {
        for (int x = 0; x <= BLOCK_WIDTH; x++) {
            map[x][y] = 1;  // 设置墙
            myprev[x][y] = nullptr;  // 置空前一个块
            vis[x][y] = 1;  // 标记为未访问
        }
    }
    map[1][1] = 0;  // 设置起点为通路
    reDarw();  // 重绘地图
    mciSendString(_T("open orb.mp3 alias bkmusic1"), NULL, 0, NULL);  // 播放背景音乐
    mciSendString(_T("play bkmusic1 repeat"), NULL, 0, NULL);  // 循环播放背景音乐

    // 遍历每个块，进行迷宫生成
    for (int i = 1; i <= BLOCK_WIDTH / 2; i++) {
        for (int j = 1; j <= BLOCK_HEIGHT / 2; j++) {
            if (i == 1 && j == 1) {  // 跳过起点
                continue;
            }
            int nx = i * 2 - 1;  // 计算奇数行列
            int ny = j * 2 - 1;
            if (!map[nx][ny]) {  // 如果该块已经是通路，跳过
                continue;
            }
            next.push({ nx, ny });  // 将该块加入栈
            vis[nx][ny] = 0;  // 标记为已访问
            blockColor(nx, ny, picked);  // 绘制当前块
            FlushBatchDraw();  // 刷新绘图

            // 生成迷宫路径
            while (map[nx][ny]) {
                int s = rand() % 4;  // 随机选择方向
                while ((!checkPath(nx + dir2[s][0], ny + dir2[s][1]))) {  // 确保路径有效
                    s = rand() % 4;
                }
                nx += dir2[s][0];  // 更新节点坐标
                ny += dir2[s][1];
                if (!vis[nx][ny]) {  // 如果遇到已访问的节点，回溯
                    while (next.top().first != nx || next.top().second != ny) {
                        vis[next.top().first][next.top().second] = 1;
                        blockColor(next.top().first, next.top().second, wal);  // 绘制墙壁
                        FlushBatchDraw();
                        next.pop();
                    }
                }
                else {  // 如果遇到新的节点
                    next.push({ nx, ny });  // 将节点加入栈
                    vis[nx][ny] = 0;  // 标记为未访问
                    if (map[nx][ny] == 1) {  // 如果是墙，继续绘制
                        blockColor(nx, ny, picked);
                        FlushBatchDraw();
                    }
                    else {  // 如果是通路，绘制通路
                        blockColor(nx, ny, road);
                        FlushBatchDraw();
                    }
                }
            }

            // 回溯并生成路径
            int lx = next.top().first, ly = next.top().second;
            vis[lx][ly] = 1;  // 标记已访问
            next.pop();  // 移除栈顶节点
            while (!next.empty()) {  // 处理栈中的剩余节点
                int nx = next.top().first, ny = next.top().second;
                vis[nx][ny] = 1;  // 标记已访问
                map[nx][ny] = 0;  // 将节点设置为通路
                map[(nx + lx) / 2][(ny + ly) / 2] = 0;  // 设置连接点为通路
                blockColor((nx + lx) / 2, (ny + ly) / 2, road);  // 绘制连接点
                blockColor(nx, ny, road);  // 绘制当前节点
                FlushBatchDraw();  // 刷新绘图
                lx = nx, ly = ny;  // 更新最后一个节点
                next.pop();  // 移除栈顶节点
            }
        }
    }
    reDarw();  // 重绘地图
    mciSendString(_T("close bkmusic1"), NULL, 0, NULL);  // 关闭背景音乐
}

// 检查给定坐标是否是有效路径
bool checkPath(int x, int y) {
    if (x <= 0 || y <= 0 || x > BLOCK_WIDTH || y > BLOCK_HEIGHT) {  // 如果越界
        return false;
    }
    return true;  // 否则有效
}
