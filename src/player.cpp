#include "player.h"
#include "maze.h"
#include "fuzzy.h"

#include <cstring>
#include <cmath>
#include <algorithm>
#include <queue>

player::player(glm::vec3 pos, int type) : ball(pos, 0.3){
    playerType = type;
    if(type == 1){
        setColor(glm::vec3(1, 0, 0));
        speed = 0.01;
    }
    else if(type == 2){
        setColor(glm::vec3(0, 0, 1));
        speed = 0.01 * 0.75;
    }
    else{
        setColor(glm::vec3(0, 1, 0));
        speed = 0.01 * 0.75;
    }
    setMaterial(M_OCEAN);
    g = std::mt19937(std::random_device()());
    direction = g() % 4;
    targetX = targetY = -1;
}

int player::getType(){
    return playerType;
}

void player::motion(int graph[MAZE_SIZE][MAZE_SIZE], bool reward[MAZE_SIZE][MAZE_SIZE], std::vector<object *> &walls, std::vector<player> &players, std::vector<std::pair<ellipsoid, bool *> > &rewards){
    // 处理玩家动作逻辑，根据玩家类型调用不同的逻辑
    switch(playerType){
        case 1:
        redMotion(graph, walls, players);
        break;
        case 2:
        blueMotion(graph, reward, walls, players, rewards);
        break;
        case 3:
        greenMotion(graph, reward, walls, players, rewards);
        break;
    }
    move(graph, walls, players);
}

void player::move(int graph[MAZE_SIZE][MAZE_SIZE], std::vector<object *> &walls, std::vector<player> &players){
    // 清除当前位置在graph中的标记
    glm::vec3 currentPos = getLoc();
    int currentX = static_cast<int>(std::round(currentPos.x));
    int currentY = static_cast<int>(std::round(currentPos.z));
    graph[currentX][currentY] = 1;

    // 移动位置
    glm::vec3 nextPos = currentPos + glm::vec3(dir[direction][0], 0, dir[direction][1]) * speed;
    setLoc(nextPos);

    // /*
    // 检查是否碰到墙壁
    if(hitWall(walls) || graph[currentX][currentY] == 0){
        // 碰到墙壁，恢复到整数位置
        setLoc(glm::vec3(static_cast<int>(std::round(currentPos.x)), currentPos.y, static_cast<int>(std::round(currentPos.z))));
        // 重新计算方向
        // std::cout << "hit wall" << std::endl;
        changeDirection(graph, walls, players);
    }
    // */

        // 更新当前位置在graph中的标记
    currentPos = getLoc();
    currentX = static_cast<int>(std::round(currentPos.x));
    currentY = static_cast<int>(std::round(currentPos.z));
    graph[currentX][currentY] = 100 + playerType;
}

bool player::hitWall(std::vector<object *> &walls){
    for(auto &w : walls){
        if(isCollide(*w)){
            return true;
        }
    }
    return false;
}

void player::changeDirection(int graph[MAZE_SIZE][MAZE_SIZE], std::vector<object *> &walls, std::vector<player> &players){
    glm::vec3 currentPos = getLoc();
    int currentX = static_cast<int>(std::round(currentPos.x));
    int currentY = static_cast<int>(std::round(currentPos.z));

    std::vector<int> possibleDirections = { 0, 1, 2, 3 };
    possibleDirections.erase(std::remove(possibleDirections.begin(), possibleDirections.end(), (direction + 2) % 4), possibleDirections.end()); // 避免回头
    possibleDirections.erase(std::remove(possibleDirections.begin(), possibleDirections.end(), direction), possibleDirections.end());

    std::shuffle(possibleDirections.begin(), possibleDirections.end(), g);

    for(int newDirection : possibleDirections){
        // std::cout << dirName[newDirection] << std::endl;
        int newX = currentX + dir[newDirection][0];
        int newY = currentY + dir[newDirection][1];

        // 检查新方向是否有路
        // std::cout << "current position: " << currentX << " " << currentY << std::endl;
        // for(int i = currentX - 1; i <= currentX + 1; ++i){
        //     for(int j = currentY - 1; j <= currentY + 1; ++j){
        //         std::cout << graph[i][j] << " ";
        //     }
        //     std::cout << std::endl;
        // }
        if(newX >= 0 && newX < MAZE_SIZE && newY >= 0 && newY < MAZE_SIZE && graph[newX][newY] != 0){
            // std::cout << "new direction: " << dirName[newDirection] << std::endl;
            direction = newDirection;
            hit = 0;
            return;
        }
    }

    // 回頭
    direction = (direction + 2) % 4;
    // std::cout << "direction: " << dirName[direction] << std::endl;
}

void player::redMotion(int graph[MAZE_SIZE][MAZE_SIZE], std::vector<object *> &walls, std::vector<player> &players){
    if(targetX == -1 && targetY == -1){
        std::uniform_int_distribution<int> distribution(0, MAZE_SIZE - 1);
        targetX = distribution(g);
        targetY = distribution(g);
        // std::cout << "=======new target: " << targetX << " " << targetY << "========" << std::endl;

        // 確保目標點是一個可以行走的點
        while(graph[targetX][targetY] == 0){
            targetX = distribution(g);
            targetY = distribution(g);
        }
    }


    // 使用BFS找到最短路徑
    std::queue<std::pair<int, int> > q;
    q.push(std::make_pair(targetX, targetY));
    std::vector<std::vector<int> > prev(MAZE_SIZE, std::vector<int>(MAZE_SIZE, -1));
    prev[targetX][targetY] = -2;
    bool found = false;
    while(!q.empty()){
        int x = q.front().first;
        int y = q.front().second;
        q.pop();
        if(x == static_cast<int>(std::round(getLoc().x)) && y == static_cast<int>(std::round(getLoc().z))){
            found = true;
            break;
        }
        for(int i = 0; i < 4; ++i){
            int newX = x + dir[i][0];
            int newY = y + dir[i][1];
            if(newX >= 0 && newX < MAZE_SIZE && newY >= 0 && newY < MAZE_SIZE && graph[newX][newY] != 0 && prev[newX][newY] == -1){
                prev[newX][newY] = i;
                q.push(std::make_pair(newX, newY));
            }
        }
    }
    // 決定下一步的方向
    if(found){
        int x = static_cast<int>(std::round(getLoc().x));
        int y = static_cast<int>(std::round(getLoc().z));
        if(x == targetX && y == targetY){
            // 到達目標點，選擇一個新的目標點
            // std::cout << "arrived at target" << std::endl;
            targetX = targetY = -1;
            return;
        }
        int newDirection = (prev[x][y] + 2) % 4;
        if(newDirection != -1){
            // std::cout << "set direction: " << dirName[newDirection] << std::endl;
            direction = newDirection;
            op = 1;
        }

    }
    else{
        // std::cout << "no path found" << std::endl;
        targetX = targetY = -1;
        op = 0;
    }
}


void player::blueMotion(int graph[MAZE_SIZE][MAZE_SIZE], bool reward[MAZE_SIZE][MAZE_SIZE], std::vector<object *> &walls, std::vector<player> &players, std::vector<std::pair<ellipsoid, bool *> > &rewards){
    if(color.y == 1 && isAlive == 1){
        speed = 0.01 * 0.75;
    }
    if(isAlive == 0){
        return;
    }
    glm::vec3 currentPos = getLoc();
    int currentX = static_cast<int>(std::round(currentPos.x));
    int currentY = static_cast<int>(std::round(currentPos.z));

    for(auto &p : players){
        if(p.getType() == 1 && isCollide(p)){
            isAlive = 0;
            graph[currentX][currentY] = 0;
            return;
        }
    }
    for(auto &r : rewards){
        if(isCollide(r.first) && *r.second){
            *r.second = false;
        }
    }
    if(reward[currentX][currentY]){
        reward[currentX][currentY] = false;
        for(auto &r : rewards){
            if(static_cast<int>(std::round(r.first.getLoc().x)) == currentX && static_cast<int>(std::round(r.first.getLoc().z)) == currentY){
                *r.second = false;
                break;
            }
        }
    }

    if(targetX == -1 || targetY == -1 || reward[targetX][targetY] == false){
        // 检查5x5范围内是否有reward，优先获取
        for(int i = -2; i <= 2; ++i){
            for(int j = -2; j <= 2; ++j){
                int checkX = currentX + i;
                int checkY = currentY + j;
                if(checkX >= 0 && checkX < MAZE_SIZE && checkY >= 0 && checkY < MAZE_SIZE && reward[checkX][checkY]){
                    targetX = checkX;
                    targetY = checkY;
                    break;
                }
            }
        }
    }

    // 如果没有附近的reward，则寻找全局的reward
    if(targetX == -1 || targetY == -1 || reward[targetX][targetY] == false){
        for(auto &r : rewards){
            if(*r.second){
                targetX = static_cast<int>(std::round(r.first.getLoc().x));
                targetY = static_cast<int>(std::round(r.first.getLoc().z));
                break;
            }
        }
    }

    // 使用BFS找到最短路径
    if(targetX != -1 && targetY != -1){
        std::queue<std::pair<int, int>> q;
        q.push(std::make_pair(targetX, targetY));
        std::vector<std::vector<int>> prev(MAZE_SIZE, std::vector<int>(MAZE_SIZE, -1));
        prev[targetX][targetY] = -2;
        bool found = false;
        while(!q.empty()){
            int x = q.front().first;
            int y = q.front().second;
            q.pop();
            if(x == currentX && y == currentY){
                found = true;
                break;
            }
            for(int i = 0; i < 4; ++i){
                int newX = x + dir[i][0];
                int newY = y + dir[i][1];
                if(newX >= 0 && newX < MAZE_SIZE && newY >= 0 && newY < MAZE_SIZE && graph[newX][newY] != 0 && prev[newX][newY] == -1 && graph[newX][newY] != 101){
                    prev[newX][newY] = i;
                    q.push(std::make_pair(newX, newY));
                }
            }
        }

        // 决定下一步的方向
        if(found){
            if(currentX == targetX && currentY == targetY){
                // 到达目标点，标记reward为已获取
                for(auto &r : rewards){
                    if(static_cast<int>(std::round(r.first.getLoc().x)) == targetX && static_cast<int>(std::round(r.first.getLoc().z)) == targetY){
                        *r.second = false;
                        break;
                    }
                }
                // 选择一个新的目标点
                targetX = targetY = -1;
                return;
            }
            int newDirection = (prev[currentX][currentY] + 2) % 4;
            if(newDirection != -1){
                direction = newDirection;
            }
        }
        else{
            // 没有找到路径，选择一个新的目标点
            targetX = targetY = -1;
        }
    }
}



void player::greenMotion(int graph[MAZE_SIZE][MAZE_SIZE], bool reward[MAZE_SIZE][MAZE_SIZE], std::vector<object *> &walls, std::vector<player> &players, std::vector<std::pair<ellipsoid, bool *> > &rewards){
    if(isAlive == 0){
        return;
    }
    int cntBlue = 0;
    for(auto &p : players){
        if(p.getType() == 2 && p.isAlive){
            cntBlue++;
        }
    }
    // std::cout << "cntBlue: " << cntBlue << std::endl;
    if(cntBlue == 0){
        playerType = 2;
        return;
    }
    glm::vec3 currentPos = getLoc();
    int currentX = static_cast<int>(std::round(currentPos.x));
    int currentY = static_cast<int>(std::round(currentPos.z));

    // 使用模糊逻辑计算移动速度
    Fuzzy speed(0.0);
    int redCount3x3 = countRedBalls(currentX, currentY, 1, graph);
    int redCount13x13 = countRedBalls(currentX, currentY, 6, graph);

    for(auto &p : players){
        if(p.getType() == 1 && isCollide(p)){
            isAlive = 0;
            graph[currentX][currentY] = 0;
            return;
        }
    }

    for(auto &r : rewards){
        if(isCollide(r.first) && *r.second){
            *r.second = false;
        }
    }
    if(reward[currentX][currentY]){
        reward[currentX][currentY] = false;
        for(auto &r : rewards){
            if(static_cast<int>(std::round(r.first.getLoc().x)) == currentX && static_cast<int>(std::round(r.first.getLoc().z)) == currentY){
                *r.second = false;
                break;
            }
        }
    }

    if(redCount3x3 > 0){
        speed = Fuzzy(1.0); // 周围3x3有红球，全速逃跑
    }
    else if(redCount13x13 > 0){
        double distanceFactor = 1.0 / (redCount13x13); // 距离越远速度越慢
        speed = Fuzzy(distanceFactor);
    }

    // 如果没有红球则停止移动
    if(redCount3x3 == 0 && redCount13x13 == 0){
        this->speed = 0;
        return;
    }
    // 選擇一個目標點 朝目標點移動
    if(targetX == -1 || targetY == -1){
        std::uniform_int_distribution<int> distribution(0, MAZE_SIZE - 1);
        targetX = distribution(g);
        targetY = distribution(g);
        while(graph[targetX][targetY] == 0){
            targetX = distribution(g);
            targetY = distribution(g);
        }
    }
    // 使用BFS找到最短路径
    std::queue<std::pair<int, int> > q;
    q.push(std::make_pair(targetX, targetY));
    std::vector<std::vector<int> > prev(MAZE_SIZE, std::vector<int>(MAZE_SIZE, -1));
    prev[targetX][targetY] = -2;
    bool found = false;
    while(!q.empty()){
        int x = q.front().first;
        int y = q.front().second;
        q.pop();
        if(x == currentX && y == currentY){
            found = true;
            break;
        }
        for(int i = 0; i < 4; ++i){
            int newX = x + dir[i][0];
            int newY = y + dir[i][1];
            if(newX >= 0 && newX < MAZE_SIZE && newY >= 0 && newY < MAZE_SIZE && graph[newX][newY] != 0 && prev[newX][newY] == -1 && graph[newX][newY] != 101){
                prev[newX][newY] = i;
                q.push(std::make_pair(newX, newY));
            }
        }
    }
    if(found){
        int x = static_cast<int>(std::round(getLoc().x));
        int y = static_cast<int>(std::round(getLoc().z));
        if(x == targetX && y == targetY){
            targetX = targetY = -1;
            return;
        }
        int newDirection = (prev[x][y] + 2) % 4;
        if(newDirection != -1){
            direction = newDirection;
        }
    }
    else{
        targetX = targetY = -1;
    }

    this->speed = speed.getValue() * 0.01 * 0.75;

}


void player::draw(){
    if(isAlive == 0){
        return;
    }
    ball::draw();
}


// 在周围一定范围内检查红球数量
int countRedBalls(int x, int y, int radius, int graph[MAZE_SIZE][MAZE_SIZE]){
    int count = 0;
    for(int i = -radius; i <= radius; ++i){
        for(int j = -radius; j <= radius; ++j){
            int checkX = x + i;
            int checkY = y + j;
            if(checkX >= 0 && checkX < MAZE_SIZE && checkY >= 0 && checkY < MAZE_SIZE && graph[checkX][checkY] == 101){
                count++;
            }
        }
    }
    return count;
}