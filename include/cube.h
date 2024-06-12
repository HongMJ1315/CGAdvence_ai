#ifndef CUBE_H
#define CUBE_H

#include "object.h"
#include <vector>   

class cube : public object{
private:
    glm::vec3 rotatePoints[8] = {};
    float l, w, h;

public:
    cube(glm::vec3, float, float, float);
    void draw();
    bool isCollide(object &);
    float getL();
    float getW();
    float getH();

};

#endif