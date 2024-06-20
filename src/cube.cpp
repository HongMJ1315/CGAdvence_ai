#include "ball.h"
#include "cube.h"
#include "material.h"
#include "GLinclude.h"

float points[][3] = { {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5},
                      {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5} };
int faces[][4] = { {0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5}, {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3} };

cube::cube(glm::vec3 loc, float l, float w, float h) :object(loc){
    std::cout << loc.x << " " << loc.y << " " << loc.z << std::endl;
    this->l = l;
    this->w = w;
    this->h = h;
}


void cube::draw(){
    if(textName != nullptr){
        SetTexture(texture, textName);
    }
    else
        SetMaterial(material, color.x, color.y, color.z);

    glPushMatrix();
    glTranslatef(loc.x, loc.y, loc.z);
    glRotatef(rotate.x, 1, 0, 0);
    glRotatef(rotate.y, 0, 1, 0);
    glRotatef(rotate.z, 0, 0, 1);
    glScalef(l, w, h);
    for(int i = 0; i < 6; i++){
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(points[faces[i][0]]);
        glTexCoord2f(10.0, 0.0);
        glVertex3fv(points[faces[i][1]]);
        glTexCoord2f(10.0, 10.0);
        glVertex3fv(points[faces[i][2]]);
        glTexCoord2f(0.0, 10.0);
        glVertex3fv(points[faces[i][3]]);
        glEnd();
    }
    glPopMatrix();
}

bool cube::isCollide(object &obj){
    cube *c = dynamic_cast<cube *>(&obj);
    if(c != nullptr){
        glm::vec2 cx = glm::vec2(loc.x - l / 2.0f, loc.x + l / 2.0f);
        glm::vec2 cy = glm::vec2(loc.y - w / 2.0f, loc.y + w / 2.0f);
        glm::vec2 cz = glm::vec2(loc.z - h / 2.0f, loc.z + h / 2.0f);
        glm::vec2 cx2 = glm::vec2(c->getLoc().x - c->getL() / 2.0f, c->getLoc().x + c->getL() / 2.0f);
        glm::vec2 cy2 = glm::vec2(c->getLoc().y - c->getW() / 2.0f, c->getLoc().y + c->getW() / 2.0f);
        glm::vec2 cz2 = glm::vec2(c->getLoc().z - c->getH() / 2.0f, c->getLoc().z + c->getH() / 2.0f);
        if(((cx.x - cx2.x < ESP && cx2.x - cx.y < ESP) || (cx2.x - cx.x < ESP && cx.x - cx2.y < ESP)) &&
            ((cy.x - cy2.x < ESP && cy2.x - cy.y < ESP) || (cy2.x - cy.x < ESP && cy.x - cy2.y < ESP)) &&
            ((cz.x - cz2.x < ESP && cz2.x - cz.y < ESP) || (cz2.x - cz.x < ESP && cz.x - cz2.y < ESP))){
            return 1;
        }
        else return 0;
    }
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
    return false;
}

float cube::getL(){
    return l;
}

float cube::getW(){
    return w;
}

float cube::getH(){
    return h;
}

