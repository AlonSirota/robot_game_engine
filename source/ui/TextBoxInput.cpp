#include "TextBoxInput.hpp"

TextBoxInput::TextBoxInput(State *state, double posX, double posY, string text, double colorR, double colorG, double colorB, double backgroundColorR, double backgroundColorG, double backgroundColorB, double width, double textSize) : TextBox(state, posX, posY, text, colorR, colorG, colorB, backgroundColorR, backgroundColorG, backgroundColorB, textSize){
    this->width = width;
}

void TextBoxInput::KeyPress(unsigned char c){
    if(this->isSelected ){
        if(c == '\b' && this->text.size() > 0){//backspace
            this->text.pop_back();
        }else{
            this->text += ((char) c);
            TextBox::calculateDimentions();
            if(this->maxX - this->minX > this->width){
                this->text.pop_back();
            }
        }
    }
}

bool TextBoxInput::MouseClick(double mX, double mY){
    if(TextBox::MouseClick(mX, mY)){
        this->isSelected = !this->isSelected;
        return true;
    };
    this->isSelected = false;
    return false;
}

void TextBoxInput::Draw(){
    TextBox::Draw();
}

void TextBoxInput::calculateDimentions(){
    TextBox::calculateDimentions();
    this->maxX = this->minX + this->width;
}