#pragma once
#include "UICommon.hpp"

#include "UIElement.hpp"
#include "TextLable.hpp"
#include "TextBox.hpp"

class TextBoxInput : public TextBox{

public:
    double backgroundColorSelected[3];

    double width;

    TextBoxInput(State *state, double posX, double posY, string text, double colorR, double colorG, double colorB, double backgroundColorR, double backgroundColorG, double backgroundColorB, double width, double textSize = 1.0);
    void KeyPress(unsigned char);
    bool MouseClick(double mX, double mY);

    void Draw();
    void calculateDimentions();

    void swapColors();
};
