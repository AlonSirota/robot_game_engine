#include "TextBoxInput.hpp"

TextBoxInput::TextBoxInput(State *state, double posX, double posY, string text, double colorR, double colorG, double colorB, double backgroundColorR, double backgroundColorG, double backgroundColorB, unsigned int size, double textSize) : TextBox(state, posX, posY, text, colorR, colorG, colorB, backgroundColorR, backgroundColorG, backgroundColorB, textSize){
    this->size = size;
    for(int i = 0; i < size; i++){
        this->text.append(" ");
    }
}

void TextBoxInput::KeyPress(unsigned char c){
    if(this->isSelected ){
        if(c == '\b' && this->actualText.size() > 0){//backspace
            this->actualText.pop_back();
        }else if(this->actualText.size() < this->size){
            this->actualText += ((char) c);
        }
        this->text.clear();
        this->text += ' ';
        this->text += this->actualText;
        for(int i = this->text.size(); i < size; i++){
            this->text.append(" ");
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
    this->text.clear();
    this->text += ' ';
    this->text += this->actualText;
    for(int i = this->text.size(); i < size; i++){
        this->text.append(" ");
    }
    TextBox::Draw();
}