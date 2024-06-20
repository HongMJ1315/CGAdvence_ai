#ifndef MAZE_H
#define MAZE_H
#define MAZE_SIZE 31
#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

void generateMaze(int graph[MAZE_SIZE][MAZE_SIZE], int maze_size);
void dfs(int graph[MAZE_SIZE][MAZE_SIZE], int x, int y, int maze_size);

#endif