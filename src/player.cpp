#include"player.h"
#include"maze.h"
#include<cstring>

player::player(glm::vec3 pos, int type):ball(pos, 0.4){
    playerType = type;
    setColor(glm::vec3(1, 0, 0));
    setMaterial(M_OCEAN);
    g = std::mt19937(std::random_device()());
    direction = g() % 4;
    for(int i = 0; i < 4; i++){
        wallColliders.push_back(cube(glm::vec3(pos.x + dir[i][0] * 0.2, 0.5, pos.z + dir[i][1] * 0.2), 0.9, 0.9, 0.9));
        wallColliders.back().setName(std::string(dirName[i]) + "WallCollider");
        roadColliders.push_back(cube(glm::vec3(pos.x + dir[i][0] * 0.4, 0.5, pos.z + dir[i][1] * 0.4), 0.9, 0.9, 0.9));
        roadColliders.back().setName(std::string(dirName[i]) + "RoadCollider");
    }
    targetX = targetY = -1;
}

void player::motion(bool graph[MAZE_SIZE][MAZE_SIZE], std::vector<object *> &walls){
    switch(playerType){
        case 1:
            redMotion(graph, walls);
            break;
        case 2:
            blueMotion(graph, walls);
            break; 
        case 3:
            greenMotion(graph, walls);
            break;
    }
}

void player::move(){
    loc += glm::vec3(dir[direction][0], 0, dir[direction][1]) * 0.01f;
    for(auto &c : wallColliders){
        c.setLoc(glm::vec3(c.getLoc().x + dir[direction][0] * 0.01f, 0.5, c.getLoc().z + dir[direction][1] * 0.01f));
    }
    for(auto &c : roadColliders){
        c.setLoc(glm::vec3(c.getLoc().x + dir[direction][0] * 0.01f, 0.5, c.getLoc().z + dir[direction][1] * 0.01f));
    }
}
bool player::checkWall(std::vector<object *> &walls, int targetX, int targetY){
    ball tmp = ball(glm::vec3(targetX, 0.5, targetY), 0.4);
    for(auto &i:walls){
        if(i->isCollide(tmp)){
            return 1;
        }
    }
    return 0;
}

void player::redMotion(bool graph[MAZE_SIZE][MAZE_SIZE], std::vector<object *> &walls){
    if(targetX != -1 && targetY != -1){
        move();
        if(loc.x - targetX < ESP && loc.z - targetY < ESP){
            targetX = targetY = -1;
        }
        return;
    }
    for(int i = 0; i < 4; i++){
        bool flag = 0;
        for(auto &w : walls){
            if(wallColliders[i].isCollide(*w) && i == direction){
                int t = (direction + 2) % 4;
                do{
                    direction = g() % 4;
                }while(wallColliders[direction].isCollide(*w) && t != direction);
                flag = 1;
                break;
            }
        }
        if(flag) break;
    }

    int cnt = 0;
    for(int i = 0; i < 4; i++){
        for(auto &w : walls){
            if(roadColliders[i].isCollide(*w)){
                cnt++;
                break;
            }
        }
    }
    // std::cout << cnt << std::endl;
    if(cnt && cnt < 2){
        std::cout << "now Loc " << loc.x << " " << loc.z << std::endl;
        do{
            direction = g() % 4;
            targetX = loc.x + dir[direction][0];
            targetY = loc.z + dir[direction][1];
            std::cout << "target: " << targetX << " " << targetY << std::endl;
        }while(checkWall(walls, targetX, targetY));
        std::cout << targetX << " " << targetY << std::endl;
        
    }
    move();
}

void player::blueMotion(bool graph[MAZE_SIZE][MAZE_SIZE], std::vector<object *> &walls){ 

    
}

void player::greenMotion(bool graph[MAZE_SIZE][MAZE_SIZE], std::vector<object *> &walls){

}

void player::draw(){
    ball::draw();
    for(auto &c : roadColliders){
        c.draw();
    }
}