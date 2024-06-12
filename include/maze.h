#ifndef MAZE_H
#define MAZE_H
#define MAZE_SIZE 15
#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

void generateMaze(bool graph[MAZE_SIZE][MAZE_SIZE], int maze_size);
void dfs(bool graph[MAZE_SIZE][MAZE_SIZE], int x, int y, int maze_size);

#endif