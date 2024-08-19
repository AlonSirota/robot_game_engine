#pragma once
#include "UICommon.hpp"

#include "UIElement.hpp"
#include "TextLable.hpp"
#include "TextBox.hpp"

class TextBoxInput : public TextBox{

public:
    string actualText;
    unsigned int size;

    TextBoxInput(State *state, double posX, double posY, string text, double colorR, double colorG, double colorB, double backgroundColorR, double backgroundColorG, double backgroundColorB, unsigned int size, double textSize = 1.0);
    void KeyPress(unsigned char);
    bool MouseClick(double mX, double mY);

    void Draw();
};
