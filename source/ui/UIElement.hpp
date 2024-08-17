#pragma once
#include "UICommon.hpp"

class UIElement{
protected:
    State *state;

public:
    UIElement(State *state, double posX, double posY){this->state = state; this->posX = posX; this->posY = posY; this->isVisible = true; this->OnClick = nullptr;};

    bool isVisible;
    bool isActive;

    double posX;
    double posY;

    void (*OnClick)(State*, double, double);

    virtual void Draw(){};
    virtual bool MouseClick(double mX, double mY){return false;};
};
