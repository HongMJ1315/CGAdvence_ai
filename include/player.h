#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"
#include "cube.h"
#include"maze.h"
#include "ball.h"
#include <vector>
#include "ellipsoid.h"

class player : public ball{
private:
    bool hit = 0, op = 0;
    const int dir[4][2] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };
    const char *dirName[4] = { "up", "right", "down", "left" };
    float speed;
    int playerType;
    int targetX, targetY;
    int direction; // 0: up, 1: right, 2: down, 3: left
    std::mt19937 g;
    void redMotion(int graph[MAZE_SIZE][MAZE_SIZE], std::vector<object *> &, std::vector<player> &); // 敵人
    void blueMotion(int graph[MAZE_SIZE][MAZE_SIZE], bool reward[MAZE_SIZE][MAZE_SIZE], std::vector<object *> &, std::vector<player> &, std::vector<std::pair<ellipsoid, bool *> > &rewards);
    void greenMotion(int graph[MAZE_SIZE][MAZE_SIZE], bool reward[MAZE_SIZE][MAZE_SIZE], std::vector<object *> &walls, std::vector<player> &players, std::vector<std::pair<ellipsoid, bool *> > &rewards);
    void move(int graph[MAZE_SIZE][MAZE_SIZE], std::vector<object *> &walls, std::vector<player> &players);
    bool hitWall(std::vector<object *> &walls);
    void changeDirection(int graph[MAZE_SIZE][MAZE_SIZE], std::vector<object *> &walls, std::vector<player> &players);
public:
    bool  isAlive = 1;
    player(glm::vec3, int);
    void motion(int graph[MAZE_SIZE][MAZE_SIZE], bool reward[MAZE_SIZE][MAZE_SIZE], std::vector<object *> &walls, std::vector<player> &players, std::vector<std::pair<ellipsoid, bool *> > &rewards); // 依照playerType決定移動方式
    void draw();
    int getType();
};

int countRedBalls(int x, int y, int radius, int graph[MAZE_SIZE][MAZE_SIZE]);
#endif