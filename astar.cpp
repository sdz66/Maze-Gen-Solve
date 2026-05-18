#include <graphics.h>  // 引入图形库
#include "initset.h"    // 引入初始化设置头文件
#include "bits/stdc++.h" // 引入标准库头文件
#pragma comment(lib,"Winmm.lib")  // 链接Winmm.lib库，用于播放音频

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

// 定义节点结构体，表示A*算法中的一个节点
struct Node {
public:
    int x;  // 节点的x坐标
    int y;  // 节点的y坐标
    int h;  // 从当前节点到目标节点的启发式距离（估计距离）
    int g;  // 从起始节点到当前节点的实际距离
    int f;  // f = g + h，用于排序选择下一个访问的节点
    bool operator<(const Node& other) const {  // 重载小于操作符，用于优先队列排序
        return f > other.f;  // 返回f值较小的节点优先
    }
};

// 外部变量声明
extern int map[BLOCK_WIDTH + 10][BLOCK_HEIGHT + 10];  // 地图数组
extern std::vector<block*> openlist;  // 开放列表
extern std::vector<block*> list;  // 已处理的节点列表
extern block* myprev[BLOCK_WIDTH + 10][BLOCK_HEIGHT + 10];  // 记录每个块的前一个块，用于路径追溯

// 函数声明
void blockColor(int x, int y, COLORREF RGB);  // 设置块的颜色
void aStar(void);  // A*算法的实现
void drawShortestPath(block* start, block* end);  // 绘制最短路径
int heuristic(block* current, block* end);  // 计算启发式函数（曼哈顿距离）

// 计算当前节点到目标节点的启发式距离，采用曼哈顿距离
int heuristic(block* current, block* end) {
    return abs(current->x - end->x) + abs(current->y - end->y);  // 返回横纵坐标的差值之和
}

// A*算法的实现
void aStar(void)
{
    // 创建一个g值矩阵，初始化为无穷大
    std::vector<std::vector<int>> g(BLOCK_WIDTH, std::vector<int>(BLOCK_HEIGHT, INT_MAX));
    std::priority_queue<Node*> openset;  // 创建一个优先队列，用于存储待处理节点
    int cnt = 1920;  // 用于调节路径颜色的计数器
    COLORREF astarPath = RGB(190, 190, cnt / 10);  // 初始路径颜色
    block* start = new block(1, 1);  // 起点
    block* end = new block(BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1);  // 终点
    Node* startNode = new Node{ start->x, start->y, 0, 0, 0 };  // 创建起点节点
    openset.push(startNode);  // 将起点节点加入优先队列
    g[start->x][start->y] = 0;  // 起点的g值为0
    mciSendString(_T("open portal.mp3 alias bkmusic2"), NULL, 0, NULL);  // 播放背景音乐
    mciSendString(_T("play bkmusic2 repeat"), NULL, 0, NULL);  // 循环播放背景音乐

    // A*算法主循环
    while (!openset.empty()) {
        Node* current = openset.top();  // 获取当前f值最小的节点
        openset.pop();  // 从优先队列中移除当前节点
        cnt++;  // 计数器增加，用于更新路径颜色
        astarPath = RGB(max(50, 190 - (cnt / 10)), max(50, 190 - cnt / 10), min(255, cnt / 10));  // 更新路径颜色
        blockColor(current->x, current->y, astarPath);  // 绘制当前节点
        FlushBatchDraw();  // 刷新绘图

        // 如果当前节点是终点，绘制最短路径并结束
        if (current->x == end->x && current->y == end->y) {
            drawShortestPath(start, end);  // 绘制最短路径
            mciSendString(_T("close bkmusic2"), NULL, 0, NULL);  // 关闭背景音乐
            blockColor(1, 1, beginPoint);  // 绘制起点颜色
            blockColor(BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1, endPoint);  // 绘制终点颜色
            FlushBatchDraw();  // 刷新绘图
            return;  // 退出算法
        }

        // 遍历当前节点的四个邻居
        for (int i = 0; i < 4; ++i) {
            int nx = current->x + dir1[i][0];  // 计算邻居的x坐标
            int ny = current->y + dir1[i][1];  // 计算邻居的y坐标
            // 检查邻居是否在地图范围内且可通行
            if (nx > 0 && ny > 0 && nx < BLOCK_WIDTH && ny < BLOCK_HEIGHT &&
                map[nx][ny] == 0 && myprev[nx][ny] == nullptr) {
                int temp = current->g + 1;  // 计算邻居的g值

                // 如果通过当前节点到达邻居的路径更短，更新邻居信息
                if (temp < g[nx][ny]) {
                    myprev[nx][ny] = new block(current->x, current->y);  // 更新邻居的前一个节点
                    g[nx][ny] = temp;  // 更新g值
                    int h = heuristic(new block(nx, ny), end);  // 计算邻居的h值
                    int f = temp + h;  // 计算邻居的f值
                    openset.push(new Node{ nx,ny,temp,h,f });  // 将邻居加入优先队列
                }
            }
        }
    }

    exit(5);  // 如果找不到路径，退出程序
}
