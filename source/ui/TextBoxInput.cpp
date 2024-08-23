#include "TextBoxInput.hpp"

TextBoxInput::TextBoxInput(State *state, double posX, double posY, string text, double colorR, double colorG, double colorB, double backgroundColorR, double backgroundColorG, double backgroundColorB, double width, double textSize) : TextBox(state, posX, posY, text, colorR, colorG, colorB, backgroundColorR, backgroundColorG, backgroundColorB, textSize){
    this->width = width;

    this->backgroundColorSelected[0] = backgroundColorR * 0.75;
    this->backgroundColorSelected[1] = backgroundColorG * 0.75;
    this->backgroundColorSelected[2] = backgroundColorB * 0.75;
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
        this->swapColors();
        return true;
    }else{
        if(this->isSelected){
            this->isSelected = false;
            this->swapColors();
        }
        return false;
    };
}

void TextBoxInput::Draw(){
    TextBox::Draw();
}

void TextBoxInput::calculateDimentions(){
    TextBox::calculateDimentions();
    this->maxX = this->minX + this->width;
}

void TextBoxInput::swapColors(){
    for(int i = 0; i < 3; i++){
        double tmp = this->backgroundColor[i];
        this->backgroundColor[i] = this->backgroundColorSelected[i];
        this->backgroundColorSelected[i] = tmp;
    }
}