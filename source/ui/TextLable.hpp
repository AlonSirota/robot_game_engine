#pragma once
#include "UICommon.hpp"
#include "UIElement.hpp"

class TextLable : public UIElement{
protected:
    string text;
    double color[3];

    double minX;
    double minY;

    double maxX;
    double maxY;

    double textSize;

    void calculateDimentions();

public:
    TextLable(State *state, double posX, double posY, string text, double colorR, double colorG, double colorB, double textSize = 1.0);
    string (*updateString)(State*); 

    void Draw();
    bool MouseClick(double mX, double mY);
};
