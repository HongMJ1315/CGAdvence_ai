#ifndef ELLIPTSOID_H
#define ELLIPTSOID_H

#include "object.h"
#include <cmath>
#include <vector>

#define INF 10000000
class ellipsoid : public object{
private:
    float l, w, h;
public:
    ellipsoid(glm::vec3, float, float, float);
    void draw();
    bool isCollide(object &);
    float getL();
    float getW();
    float getH();
};

#endif