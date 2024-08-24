#pragma once

#include "UICommon.hpp"

#include "UIElement.hpp"
#include "TextLable.hpp"
#include "TextBox.hpp"

class SolidColor : public UIElement{
protected:
    double minX;
    double minY;

    double maxX;
    double maxY;

public:
    double color[4];

    SolidColor(State *state, double posX, double posY, double width, double hight, double colorR, double colorG, double colorB, double alpha);
    bool MouseClick(double mX, double mY);

    void Draw();
};

class SolidColorCircle : public UIElement{
protected:
    double rad;
public:
    double color[4];

    SolidColorCircle(State *state, double posX, double posY, double rad, double colorR, double colorG, double colorB, double alpha);
    bool MouseClick(double mX, double mY);
    void Draw();
};

class SolidColorTriangle : public UIElement{
protected:
    double side;
public:
    double color[4];

    SolidColorTriangle(State *state, double posX, double posY, double side, double colorR, double colorG, double colorB, double alpha);
    bool MouseClick(double mX, double mY);
    void Draw();
};