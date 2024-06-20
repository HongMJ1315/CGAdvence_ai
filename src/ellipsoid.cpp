#include "ellipsoid.h"
#include "cube.h"
#include "material.h"
#include "GLinclude.h"
#include "ball.h"

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
    ball *b = dynamic_cast<ball *>(&obj);
    if(b != nullptr){
        glm::vec2 bx = glm::vec2(b->getLoc().x - b->getR(), b->getLoc().x + b->getR());
        glm::vec2 by = glm::vec2(b->getLoc().y - b->getR(), b->getLoc().y + b->getR());
        glm::vec2 bz = glm::vec2(b->getLoc().z - b->getR(), b->getLoc().z + b->getR());
        glm::vec2 cx = glm::vec2(loc.x - l / 2.0f, loc.x + l / 2.0f);
        glm::vec2 cy = glm::vec2(loc.y - w / 2.0f, loc.y + w / 2.0f);
        glm::vec2 cz = glm::vec2(loc.z - h / 2.0f, loc.z + h / 2.0f);
        if(((bx.x - cx.x < ESP && cx.x - bx.y < ESP) || (cx.x - bx.x < ESP && bx.x - cx.y < ESP)) &&
            ((by.x - cy.x < ESP && cy.x - by.y < ESP) || (cy.x - by.x < ESP && by.x - cy.y < ESP)) &&
            ((bz.x - cz.x < ESP && cz.x - bz.y < ESP) || (cz.x - bz.x < ESP && bz.x - cz.y < ESP))){
            return 1;
        }
        else return 0;
    }
}
