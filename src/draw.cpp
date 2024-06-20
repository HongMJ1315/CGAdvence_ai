#include "draw.h"
#include "ellipsoid.h"
#include <algorithm>
#include "maze.h"
#include "player.h"
#define BOX_SIZE 11
#define REWARD 10

int keyState[GLFW_KEY_LAST] = { 0 };
int directionKey[4] = { 0 };
bool phyActive = false;
unsigned int textName[10];
unsigned char floorText[TEXTURE_SIZE][TEXTURE_SIZE][4];
unsigned char emptyText[TEXTURE_SIZE][TEXTURE_SIZE][4];
unsigned char wallText[TEXTURE_SIZE][TEXTURE_SIZE][4];
int viewMode = 0;
std::vector<player> players;
std::vector<object *> floors;
std::vector<object *> walls;
std::vector<std::pair<ellipsoid, bool *> > rewards;
bool showObject = true;
int graph[MAZE_SIZE][MAZE_SIZE] = {};
bool reward[MAZE_SIZE][MAZE_SIZE] = {};



void initTexture(){
    for(int i = 0; i < TEXTURE_SIZE; i++)
        for(int j = 0; j < TEXTURE_SIZE; j++)
            emptyText[i][j][0] = emptyText[i][j][1] =
            emptyText[i][j][2] = emptyText[i][j][3] = 255;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 10);
    glGenTextures(10, textName);
    GenerateFloorTexture(floorText);
    GenerateWallTexture(wallText);
    TextureInit(TEXTURE::T_FLOOR, textName, floorText, TEXTURE_SIZE, TEXTURE_SIZE);
    TextureInit(TEXTURE::T_WALL, textName, wallText, TEXTURE_SIZE, TEXTURE_SIZE);
    // TextureInit(TEXTURE::T_WALL, textName, emptyText, TEXTURE_SIZE, TEXTURE_SIZE);
    setGLSLshaders("shader/Phong.vert", "shader/Phong.frag");
    int texLoc = glGetUniformLocation(ReturnProgramID(), "myTex");
    // std::cout << texLoc << std::endl;
    glUniform1i(texLoc, 0);
}
void glInit(){
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    //smooth shading
    glShadeModel(GL_SMOOTH);
}

void setLight(){
    glEnable(SUN_LIGHT);
    GLfloat light_position[] = { 0.0, -1.0, 0.0, 0.0 };
    GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat light_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat light_specular[] = { 0.5, 0.5, 0.5, 1.0 };
    glLightfv(SUN_LIGHT, GL_POSITION, light_position);
    glLightfv(SUN_LIGHT, GL_AMBIENT, light_ambient);
    glLightfv(SUN_LIGHT, GL_DIFFUSE, light_diffuse);
    glLightfv(SUN_LIGHT, GL_SPECULAR, light_specular);
}

void initObjects(int num){
    generateMaze(graph, MAZE_SIZE);
    for(int i = 0; i < MAZE_SIZE; i++){
        for(int j = 0; j < MAZE_SIZE; j++){
            std::cout << graph[i][j] << " ";
        }
        std::cout << std::endl;
    }
    for(int i = 0; i < REWARD; i++){
        int x = rand() % MAZE_SIZE;
        int y = rand() % MAZE_SIZE;
        if(graph[x][y] == 1){
            reward[x][y] = 1;
            rewards.push_back(std::make_pair(ellipsoid(glm::vec3(x, 1, y), 0.1, 0.5, 0.5), &reward[x][y]));
            rewards.back().first.setMaterial(M_OCEAN);
            rewards.back().first.setName("Reward");
            rewards.back().first.setColor(glm::vec3(0, 1, 1));
            rewards.back().first.setRotateV(glm::vec3(0, 20, 0));
        }
        else i--;
    }
    float transX = 0, transY = -0;
    for(int i = 0; i < MAZE_SIZE; i++){
        for(int j = 0; j < MAZE_SIZE; j++){
            floors.push_back(new cube(glm::vec3(i + transX, 0, j + transY), 1, 1, 1));
            floors.back()->setTexture(TEXTURE::T_FLOOR, textName);
            floors.back()->setMaterial(M_OCEAN);
            floors.back()->setName("Floor");
            if(!graph[i][j]){
                walls.push_back(new cube(glm::vec3(i + transX, 1, j + transY), 1, 1, 1));
                walls.back()->setTexture(TEXTURE::T_WALL, textName);
                walls.back()->setName("Wall");
                walls.back()->setMaterial(M_OCEAN);
            }
        }
    }
    players.push_back(player(glm::vec3(MAZE_SIZE / 2 + 1, 1, MAZE_SIZE / 2 + 1), 1));
    players.back().setName("Red");
    players.push_back(player(glm::vec3(MAZE_SIZE / 2 - 1, 1, MAZE_SIZE / 2 + 1), 1));
    players.back().setName("Red");
    players.push_back(player(glm::vec3(MAZE_SIZE / 2 + 1, 1, MAZE_SIZE / 2 - 1), 1));
    players.back().setName("Red");
    players.push_back(player(glm::vec3(MAZE_SIZE / 2 - 1, 1, MAZE_SIZE / 2 - 1), 1));
    players.back().setName("Red");
    players.push_back(player(glm::vec3(1, 1, 1), 2));
    players.back().setName("Blue");
    players.push_back(player(glm::vec3(1, 1, MAZE_SIZE - 2), 2));
    players.back().setName("Blue");
    players.push_back(player(glm::vec3(MAZE_SIZE - 2, 1, MAZE_SIZE - 2), 2));
    players.back().setName("Blue");
    players.push_back(player(glm::vec3(MAZE_SIZE - 2, 1, 1), 2));
    players.back().setName("Blue");
    for(int i = 0; i < 4; i++){
        // 在1-10或 20-30之間生成綠球
        int x, y;
        do{
            x = rand() % 5 + 1;
            y = rand() % 5 + 1;
            int t = rand() % 2;
            if(i % 2 == 0){
                x += 20;
                y += 20;
            }
        }while(graph[x][y] == 0);
        players.push_back(player(glm::vec3(x, 1, y), 3));
        players.back().setName("Green");
    }
}

void move(float dx, float dy, float dz, glm::vec3 &frontPos, glm::vec3 &cameraPos){
    frontPos.x += dx;
    frontPos.y += dy;
    frontPos.z += dz;
    cameraPos.x += dx;
    cameraPos.y += dy;
    cameraPos.z += dz;
}

void update(glm::vec3 &frontPos, glm::vec3 &cameraPos){
    // front and back
    glm::vec3 dl = glm::normalize(glm::vec3(frontPos.x, 0, frontPos.z) - glm::vec3(cameraPos.x, 0, cameraPos.z)) * MOVE_SPEED;
    // right and left
    glm::vec3 dr = glm::normalize(glm::cross(frontPos - cameraPos, glm::vec3(0, 1, 0))) * MOVE_SPEED;
    if(keyState['W']){
        cameraPos += dl * MOVE_SPEED;
        frontPos += dl * MOVE_SPEED;
    }
    if(keyState['S']){
        cameraPos -= dl * MOVE_SPEED;
        frontPos -= dl * MOVE_SPEED;
    }
    // Left
    if(keyState['A']){
        cameraPos -= dr * MOVE_SPEED;
        frontPos -= dr * MOVE_SPEED;
    }
    // Right
    if(keyState['D']){
        cameraPos += dr * MOVE_SPEED;
        frontPos += dr * MOVE_SPEED;
    }


    // Rotate
    if(keyState['I']){
        frontPos = moveCameraUD(cameraPos, frontPos, -ROTATE_SPEED);
    }
    if(keyState['K']){
        frontPos = moveCameraUD(cameraPos, frontPos, ROTATE_SPEED);
    }
    if(keyState['J']){
        frontPos = moveCameraLR(cameraPos, frontPos, -ROTATE_SPEED);
    }
    if(keyState['L']){
        frontPos = moveCameraLR(cameraPos, frontPos, ROTATE_SPEED);
    }

    if(directionKey[0]){
        cameraPos.y += MOVE_SPEED;
        frontPos.y += MOVE_SPEED;
    }
    if(directionKey[1]){
        cameraPos.y -= MOVE_SPEED;
        frontPos.y -= MOVE_SPEED;
    }
}

void drawCoordinateString(glm::vec3 cameraPos, glm::vec3 frontPos, int width, int height, float dt, int fps){
    glUseProgram(0);
    std::string cameraPosSt = "Camera Position: (" + std::to_string(cameraPos.x) + ", " + std::to_string(cameraPos.y) + ", " + std::to_string(cameraPos.z) + ")";
    // std::string frontPosSt = "Front Position: (" + std::to_string(frontPos.x) + ", " + std::to_string(frontPos.y) + ", " + std::to_string(frontPos.z) + ")";
    std::string dtFpsSt = "dt: " + std::to_string(dt) + " fps: " + std::to_string(fps);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glViewport(0, 0, width, height);
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glPushAttrib(GL_CURRENT_BIT);
    // Set text color
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glRasterPos2f(10, height - 30);
    for(int i = 0; i < cameraPosSt.length(); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, cameraPosSt[i]);
    }
    int nowHeight = height - 60;
    glRasterPos2f(10, nowHeight);
    for(int i = 0; i < dtFpsSt.length(); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, dtFpsSt[i]);
    }
    glPopAttrib();
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void drawSingleView(int width, int height, glm::vec3 &cameraPos, glm::vec3 &frontPos){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);
    glEnable(GL_LIGHTING);
    gluPerspective(60.0f, (float) width / (float) height, 0.1f, 10000.0f);
    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, frontPos.x, frontPos.y, frontPos.z, 0.0f, 1.0f, 0.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    setLight();
    for(auto &obj : floors){

        obj->draw();
    }
    for(auto &obj : walls){
        obj->draw();
    }
    for(auto &p : players){
        p.draw();
    }
    for(auto &r : rewards){
        if(*r.second){
            r.first.draw();
        }
    }


}



void display(GLFWwindow *window, int width, int height, float dt, int fps, glm::vec3 &cameraPos, glm::vec3 &frontPos){
    // glUseProgram(0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    drawCoordinateString(cameraPos, frontPos, width, height, dt, fps);

    glUseProgram(ReturnProgramID());
    if(viewMode == 0)
        drawSingleView(width, height, cameraPos, frontPos);
    glfwSwapBuffers(window);
    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        std::cerr << std::hex << "OpenGL error: " << error << std::endl;
    }
}


void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods){
    if(action == GLFW_PRESS){
        keyState[key] = true;
        if(key == GLFW_KEY_P){
            phyActive = !phyActive;
        }
        if(key == GLFW_KEY_V){
            viewMode = (viewMode + 1) % 2;
        }
        if(key == GLFW_KEY_O){
            showObject = !showObject;
        }
        switch(key){
            case GLFW_KEY_UP:
            directionKey[0] = true;
            break;
            case GLFW_KEY_DOWN:
            directionKey[1] = true;
            break;
            case GLFW_KEY_LEFT:
            directionKey[2] = true;
            break;
            case GLFW_KEY_RIGHT:
            directionKey[3] = true;
            break;

        }
    }
    else if(action == GLFW_RELEASE){
        keyState[key] = false;
        switch(key){
            case GLFW_KEY_UP:
            directionKey[0] = false;
            break;
            case GLFW_KEY_DOWN:
            directionKey[1] = false;
            break;
            case GLFW_KEY_LEFT:
            directionKey[2] = false;
            break;
            case GLFW_KEY_RIGHT:
            directionKey[3] = false;
            break;
        }
    }
}

void updatePhysics(float dt, float t){
    int cntReward = 0;
    for(auto &r : rewards){
        if(*r.second){
            cntReward++;
        }
    }
    if(cntReward == 0){
        std::cout << "Win!" << std::endl;
        exit(0);
    }
    int cntAlive = 0;
    for(auto &p : players){
        if(p.isAlive && p.getType() != 1){
            cntAlive++;
        }
    }
    if(cntAlive == 0){
        std::cout << "Lose!" << std::endl;
        exit(0);
    }
    for(auto &p : players){
        p.motion(graph, reward, walls, players, rewards);
    }
    // for(int i = 0; i < MAZE_SIZE; i++){
    //     for(int j = 0; j < MAZE_SIZE; j++){
    //         std::cout << graph[i][j] << " ";
    //     }
    // }
    for(auto &r : rewards){
        if(*r.second){
            r.first.update(dt);
        }
    }
}
