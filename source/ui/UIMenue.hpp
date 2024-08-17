#pragma once
#include "UICommon.hpp"

#include "UIElement.hpp"
#include "TextLable.hpp"
#include "TextBox.hpp"
#include "SolidColor.hpp"

class UIMenue : public UIElement{
protected:
    std::list<UIElement*> elements;

public:
    UIMenue(State *);
    ~UIMenue();

    void Draw();
    bool MouseClick(double mX, double mY);
    void AddUIElement(UIElement *element);
};
