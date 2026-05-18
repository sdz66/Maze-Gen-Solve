# 随机迷宫生成与路径搜索算法的图形化实现

一个基于 C++ 和 EasyX 图形库的迷宫生成与求解可视化程序，支持多种迷宫生成算法和路径搜索算法，并配有实时动画演示与音效。


<img width="406" height="228" alt="2" src="https://github.com/user-attachments/assets/03fc74a3-8cb3-474c-a773-2581548352dc" />


<img width="408" height="228" alt="3" src="https://github.com/user-attachments/assets/1ac93f99-9c54-4a29-92fe-5dd27d12507d" />


## 功能特性

### 迷宫生成算法（5 种）

| 算法 | 说明 |
|------|------|
| **随机化 Prim 算法** | 基于最小生成树思想，从起点随机扩展墙壁列表，逐步打通通道 |
| **随机化 Kruskal 算法** | 基于并查集（Union-Find），随机合并不同集合的单元格 |
| **深度优先搜索（DFS）** | 基于递归回溯，利用栈结构深度优先地探索并打通墙壁 |
| **十字递归划分算法** | 递归地用水平和垂直墙壁将区域一分为四，随机打通三面墙 |
| **Wilson 算法** | 基于环形擦除随机游走（Loop-Erased Random Walk），生成均匀迷宫 |

### 路径搜索算法（3 种）

| 算法 | 说明 | 特点 |
|------|------|------|
| **DFS 深度优先搜索** | 利用栈结构沿一条路径深入探索 | 可能找不到最短路径 |
| **BFS 广度优先搜索** | 利用队列结构逐层扩展搜索 | 保证找到最短路径 |
| **A\* 算法** | 结合曼哈顿距离启发式函数，优先扩展 f = g + h 最小的节点 | 高效找到最短路径 |

> **推荐组合**：Prim / Kruskal / DFS 生成 + DFS / A\* 求解；十字划分 / Wilson 生成 + BFS / A\* 求解

## 技术细节

- **语言**：C++14
- **图形库**：[EasyX](https://easyx.cn/)（`graphics.h`）
- **音频**：Windows Multimedia API（`Winmm.lib`，`mciSendString`）
- **构建工具**：CMake / Visual Studio
- **画布分辨率**：1920 x 1080，单元格大小 12 x 12 像素
- **起点**：左上角 `(1, 1)`，绿色
- **终点**：右下角 `(159, 89)`，红色
- **最短路径**：黄色高亮

## C++ 知识点应用

### 数据结构

| 知识点 | 应用位置 |
|--------|----------|
| `std::vector` | Prim / Kruskal 的候选墙列表、DFS 生成的可扩展列表 |
| `std::stack` | DFS 路径搜索的探索栈、Wilson 算法的路径回溯栈 |
| `std::queue` | BFS 路径搜索的层级队列 |
| `std::priority_queue` | A\* 算法的开放节点优先队列（按 f 值排序） |
| `std::pair` | Wilson 箐法中存储坐标对 |
| 二维数组 | 地图存储（`map[][]`）、前驱节点记录（`myprev[][]`）、访问标记（`vis[][]`） |
| 并查集（Union-Find） | Kruskal 算法中判断两个单元格是否属于同一集合，含路径压缩优化 |

### 面向对象

| 知识点 | 应用位置 |
|--------|----------|
| `struct` 与构造函数 | `block` 结构体表示坐标单元格，`Node` 结构体表示 A\* 节点 |
| 运算符重载 | `Node::operator<` 重载，使 `std::priority_queue` 按 f 值升序排列 |

### 指针与内存

| 知识点 | 应用位置 |
|--------|----------|
| 指针与指针数组 | `block* myprev[][]` 记录路径前驱，用于回溯最短路径 |
| 动态内存分配 | `new block(...)` 动态创建节点对象 |
| `nullptr` | 初始化与空值判断 |

### 预处理器与编译

| 知识点 | 应用位置 |
|--------|----------|
| `#pragma once` | 头文件保护，防止重复包含 |
| `#pragma comment(lib, ...)` | 链接 `Winmm.lib` 音频库 |
| `#define` 宏 | 定义颜色常量（`road`、`wal`、`beginPoint` 等） |
| `enum` 枚举 | 定义画布尺寸等全局常量（`WIDTH`、`HEIGHT`、`BLOCK_XY`） |
| `const` 常量数组 | 定义四方向偏移量（`dir1`、`dir2`） |
| `extern` 声明 | 多文件共享全局变量（`map`、`openlist`、`myprev` 等） |

### STL 算法与随机数

| 知识点 | 应用位置 |
|--------|----------|
| `std::shuffle` | 十字递归划分中随机打乱打通墙壁的顺序 |
| `std::mt19937` + `std::random_device` | C++11 随机数引擎，配合 `shuffle` 使用 |
| `srand` / `rand` | Prim、Kruskal、DFS 生成中的随机选择 |

### 算法设计

| 知识点 | 应用位置 |
|--------|----------|
| 递归 | 十字递归划分算法（`crossDivide`）将区域层层分割 |
| 回溯思想 | DFS 迷宫生成中的递归回溯、Wilson 算法中的环形擦除 |
| 启发式搜索 | A\* 算法使用曼哈顿距离作为启发函数 `h(n)` |
| 多文件组织 | 各算法独立为 `.cpp` 文件，通过 `extern` 共享全局状态 |

## 项目结构

```
├── Main.cpp          # 程序入口、主菜单、画布初始化与全局绘制函数
├── initset.h         # 全局常量定义（画布尺寸、单元格大小、方向偏移、颜色）
├── prim.cpp          # 随机化 Prim 迷宫生成
├── kruskal.cpp       # 随机化 Kruskal 迷宫生成（含并查集）
├── dfs.cpp           # 路径搜索 —— DFS
├── dfsmap.cpp        # 迷宫生成 —— DFS（递归回溯）
├── bfs.cpp           # 路径搜索 —— BFS（含最短路径绘制）
├── astar.cpp         # 路径搜索 —— A*（含启发式函数与最短路径绘制）
├── devide.cpp        # 十字递归划分迷宫生成
├── wilson.cpp        # Wilson 迷宫生成（环形擦除随机游走）
├── orb.mp3           # 迷宫生成阶段背景音效
├── portal.mp3        # 路径搜索阶段背景音效
├── levelup.mp3       # 完成提示音效
├── 1.jpg             # 效果截图
├── CMakeLists.txt    # CMake 构建配置
└── ds project.sln    # Visual Studio 解决方案
```


<img width="460" height="258" alt="1" src="https://github.com/user-attachments/assets/378853a1-f33c-456a-8096-1ce6bd5ee79d" />


## 构建与运行

### 环境要求

- Windows 操作系统
- Visual Studio 2019+（需安装 C++ 桌面开发工作负载）
- [EasyX 图形库](https://easyx.cn/)（安装后自动集成到 Visual Studio）

### 使用 Visual Studio

1. 用 Visual Studio 打开 `ds project.sln`
2. 选择 `Release` 或 `Debug` 配置
3. 按 `Ctrl + F5` 编译运行

### 使用 CMake + CLion

1. 确保已安装 EasyX 并正确配置头文件和库路径
2. 用 CLion 打开项目目录
3. 加载 CMake 配置后编译运行

## 使用方法

1. 启动程序后，控制台显示菜单，选择一种**迷宫生成算法**（输入 1-5）
2. 接着选择一种**路径搜索算法**（输入 1-3）
3. 程序自动打开图形窗口，先以动画形式生成迷宫
4. 迷宫生成完毕后，自动以动画形式搜索从左上角到右下角的路径
5. 搜索完成后，黄色线条标出找到的路径
