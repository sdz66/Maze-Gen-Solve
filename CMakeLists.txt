cmake_minimum_required(VERSION 4.1)
project(project)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(project
        astar.cpp
        bfs.cpp
        devide.cpp
        dfs.cpp
        dfsmap.cpp
        initset.h
        kruskal.cpp
        Main.cpp
        prim.cpp
        wilson.cpp)
