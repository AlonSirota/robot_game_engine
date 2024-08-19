#pragma once
#include "UICommon.hpp"

class UIElement{
protected:
    State *state;

public:
    UIElement(State *state, double posX, double posY){this->state = state; this->posX = posX; this->posY = posY; this->isVisible = true; this->isSelected = false; this->OnClick = nullptr; this->update = nullptr; this->onKey = nullptr;};

    bool isVisible;
    bool isActive;

    bool isSelected;

    double posX;
    double posY;

    void (*OnClick)(State*, double, double);

    void (*update)(UIElement*, State*);

    void (*onKey)(UIElement*, State*, unsigned char);

    virtual void Draw(){if(this->update != nullptr) this->update(this, this->state);};
    virtual bool MouseClick(double mX, double mY){return false;};
    virtual void KeyPress(unsigned char){if(isSelected && this->onKey!=nullptr) this->update(this, this->state);};
};
