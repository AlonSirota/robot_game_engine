#pragma once
#include "UICommon.hpp"

#include "UIElement.hpp"
#include "TextLable.hpp"
#include "TextBox.hpp"
#include "SolidColor.hpp"

#include "UIDebugInfo.hpp"
#include "UIMenue.hpp"
#include "MainMenue.hpp"
#include "HelpMenue.hpp"
#include "OptionsMenue.hpp"



class UIManager{
private:
    std::list<UIElement*> elements;
    State *state;

    UIElement *mainMenue;
    UIElement *helpMenue;
    UIElement *optionsMenue;
    

public:
    UIManager(State* state);
    ~UIManager();
    
    void Draw();
    void AddUIElement(UIElement *element);
    bool MouseClick(double mX, double mY);
};