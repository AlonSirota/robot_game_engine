#pragma once
#include "UICommon.hpp"

#include "UIElement.hpp"
#include "TextLable.hpp"

class TextBox : public TextLable{
protected:
    double backgroundColor[3];

public:
    TextBox(State *state, double posX, double posY, string text, double colorR, double colorG, double colorB, double backgroundColorR, double backgroundColorG, double backgroundColorB, double textSize = 1.0);
    void Draw();

};
