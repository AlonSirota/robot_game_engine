#include "SolidColor.hpp"

SolidColor::SolidColor(State *state, double posX, double posY, double width, double hight, double colorR, double colorG, double colorB, double alpha = 0):UIElement(state, posX, posY){
    this->color[0] = colorR;
    this->color[1] = colorG;
    this->color[2] = colorB;
    this->color[3] = alpha;

    this->minX = this->posX;
    this->minY = this->posY;
    
    this->maxX = this->minX + width;
    this->maxY = this->minY + hight;
};

void SolidColor::Draw(){
    glColor4d(this->color[0], this->color[1], this->color[2], this->color[3]);
    glRectf(this->minX, this->minY, this->maxX, this->maxY);
}
