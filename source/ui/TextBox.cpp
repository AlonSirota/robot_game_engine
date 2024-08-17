#include "TextBox.hpp"

TextBox::TextBox(State *state, double posX, double posY, string text, double colorR, double colorG, double colorB, double backgroundColorR, double backgroundColorG, double backgroundColorB, double textSize):TextLable(state, posX, posY, text, colorR, colorG, colorB, textSize){
    this->backgroundColor[0] = backgroundColorR;
    this->backgroundColor[1] = backgroundColorG;
    this->backgroundColor[2] = backgroundColorB;
};

void TextBox::Draw(){
    TextLable::Draw();
    this->calculateDimentions();
    glColor3d(this->backgroundColor[0], this->backgroundColor[1], this->backgroundColor[2]);
    glRectf(this->minX, this->minY, this->maxX, this->maxY);
};
