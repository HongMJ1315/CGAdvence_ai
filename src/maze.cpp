#include "maze.h"
#include <random>
#include <ctime>

// 0: wall, 1: path
void generateMaze(int graph[MAZE_SIZE][MAZE_SIZE], int maze_size) {
    // Initialize all cells as walls
    for (int i = 0; i < maze_size; ++i) {
        for (int j = 0; j < maze_size; ++j) {
            graph[i][j] = false;
        }
    }
    
    // Start the DFS from the top-left corner
    dfs(graph, 1, 1, maze_size);
    for(int i = 0; i < MAZE_SIZE; i++){
        graph[i][0] =
        graph[i][MAZE_SIZE - 1] = 
        graph[0][i] = 
        graph[MAZE_SIZE - 1][i] = 0;
    }
    for(int i = 1; i <= MAZE_SIZE - 2; i++){
        graph[i][1] =
        graph[i][MAZE_SIZE - 2] = 
        graph[1][i] = 
        graph[MAZE_SIZE - 2][i] = 1;
    }
    int cx = (MAZE_SIZE  - 1)/ 2;
    int cy = (MAZE_SIZE  - 1)/ 2;
    for(int i = cx - 1; i <= cx + 1; i++){
        for(int j = cy - 1; j <= cy + 1; j++){
            graph[i][j] = 1;
        }
    }
}

void dfs(int graph[MAZE_SIZE][MAZE_SIZE], int x, int y, int maze_size) {
    static const int DX[] = { 0, 1, 0, -1 };
    static const int DY[] = { 1, 0, -1, 0 };
    std::vector<int> directions = { 0, 1, 2, 3 };
    std::random_device rd;
    std::mt19937 g(rd());
    // Shuffle directions to ensure randomness
    std::shuffle(directions.begin(), directions.end(),g);

    for (int i = 0; i < 4; ++i) {
        int nx = x + DX[directions[i]] * 2;
        int ny = y + DY[directions[i]] * 2;

        if (nx >= 0 && nx < maze_size && ny >= 0 && ny < maze_size && !graph[nx][ny]) {
            graph[x + DX[directions[i]]][y + DY[directions[i]]] = true; // Knock down the wall
            graph[nx][ny] = true; // Mark the new cell as part of the maze
            dfs(graph, nx, ny, maze_size);
        }
    }
}