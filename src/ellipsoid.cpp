#include "ellipsoid.h"
#include "cube.h"
#include "material.h"
#include "GLinclude.h"

ellipsoid::ellipsoid(glm::vec3 loc, float l, float w, float h) :object(loc){
    this->l = l;
    this->w = w;
    this->h = h;
}

void ellipsoid::draw(){
    if(textName != nullptr){
        SetTexture(texture, textName);
    }
    else
        SetMaterial(material, color.x, color.y, color.z);
    glPushMatrix();
    glTranslatef(object::getLoc().x, object::getLoc().y, object::getLoc().z);
    glRotatef(rotate.x, 1, 0, 0);
    glRotatef(rotate.y, 0, 1, 0);
    glRotatef(rotate.z, 0, 0, 1);
    glScalef(this->l, this->w, this->h);
    glutSolidSphere(1, 20, 20);
    glPopMatrix();
}

float ellipsoid::getL(){
    return l;
}

float ellipsoid::getW(){
    return w;
}

float ellipsoid::getH(){
    return h;
}

bool ellipsoid::isCollide(object &obj){
    return 0;
}
