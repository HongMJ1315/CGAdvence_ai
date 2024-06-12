#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"
#include "cube.h"
#include"maze.h"
#include "ball.h"
#include <vector>

class player : public ball{
    private:
    const int dir[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    const char *dirName[4] = {"up", "right", "down", "left"};
    int tmp1 = 0;
    int playerType;
    float targetX, targetY;
    int direction; // 0: up, 1: right, 2: down, 3: left
    std::mt19937 g;
    std::vector<cube> wallColliders; // 在四個方向上的碰撞器
    std::vector<cube> roadColliders; // 在四個方向上的碰撞器
    void redMotion(bool graph[MAZE_SIZE][MAZE_SIZE],std::vector<object *>&); // 敵人
    void blueMotion(bool graph[MAZE_SIZE][MAZE_SIZE],std::vector<object *>&);
    void greenMotion(bool graph[MAZE_SIZE][MAZE_SIZE],std::vector<object *>& );
    void move();
    bool checkWall(std::vector<object *> &walls, int , int);

    public:
    player(glm::vec3, int);
    void motion(bool graph[MAZE_SIZE][MAZE_SIZE], std::vector<object *> &walls); // 依照playerType決定移動方式
    void draw();
};

#endif