#pragma once

#include "UICommon.hpp"

#include "UIElement.hpp"
#include "TextLable.hpp"
#include "TextBox.hpp"

class SolidColor : public UIElement{
protected:
    double color[4];

    double minX;
    double minY;

    double maxX;
    double maxY;

public:
    SolidColor(State *state, double posX, double posY, double width, double hight, double colorR, double colorG, double colorB, double alpha);

    void Draw();
};
