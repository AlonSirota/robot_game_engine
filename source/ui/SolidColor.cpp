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
    UIElement::Draw();
    glColor4d(this->color[0], this->color[1], this->color[2], this->color[3]);
    glRectf(this->minX, this->minY, this->maxX, this->maxY);
}

bool SolidColor::MouseClick(double mX, double mY){
    bool isInside = mX >= this->minX && mX <= this->maxX && mY >= this->minY && mY <= this->maxY;

    if(isInside && this->OnClick != nullptr){
        this->OnClick(this->state, mX, mY);
    }

    return isInside;
}

void DrawCircle(float x, float y, float radios, float width, double borderColor[], double infilColor[], UINT samples){
    //begin drawing circle 
    glColor4d(borderColor[0], borderColor[1], borderColor[2], borderColor[3]);
    glLineWidth(width);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < samples; i++){
        float theta = 2.0f * 3.1415926f * float(i) / float(samples);
        float curX = radios * cosf(theta);//calculate the x component
        float curY = radios * sinf(theta);//calculate the y component
        glVertex2f(x + curX, y + curY);//output vertex
    };
    glEnd();

    //infill
    glColor4d(infilColor[0], infilColor[1], infilColor[2], infilColor[3]);
    glLineWidth(0);
    radios = radios - width/2.2;
    glBegin(GL_POLYGON);
        for (int i = 0; i < samples; i++){
            float theta = 2.0f * 3.1415926f * float(i) / float(samples);
            float curX = radios * cosf(theta);//calculate the x component
            float curY = radios * sinf(theta);//calculate the y component
            glVertex2f(x + curX, y + curY);//output vertex
        };
    glEnd();
}

SolidColorCircle::SolidColorCircle(State *state, double posX, double posY, double rad, double colorR, double colorG, double colorB, double alpha = 0):UIElement(state, posX, posY){
    this->color[0] = colorR;
    this->color[1] = colorG;
    this->color[2] = colorB;
    this->color[3] = alpha;

    this->rad = rad;
}

void SolidColorCircle::Draw(){
    UIElement::Draw();
    glColor4d(this->color[0], this->color[1], this->color[2], this->color[3]);
    DrawCircle(this->posX, this->posY, this->rad, 1, this->color, this->color, 100);
}

bool SolidColorCircle::MouseClick(double mX, double mY){
    bool isInside = (pow(mX - this->posX, 2) + pow(mY - this->posY, 2)) <= pow(this->rad, 2);

    if(isInside && this->OnClick != nullptr){
        this->OnClick(this->state, mX, mY);
    }

    return isInside;
}
