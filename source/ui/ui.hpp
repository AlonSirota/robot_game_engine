#pragma once
#include <GL/freeglut_std.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <algorithm>
#include <list>
#include "../State.h"
#include <string>

using std::string;

class UIElement{
protected:
    State *state;
public:
    UIElement(State *state, double posX, double posY){this->state = state; this->posX = posX; this->posY = posY; this->isVisible = true;};

    bool isVisible;
    double posX;
    double posY;

    virtual void Draw(){};
    virtual bool IsInside(double x, double y){return false;};
    virtual void OnClick(double mX, double mY){};
};

class TextLable : public UIElement{
protected:
    string text;
    double color[3];
public:
    TextLable(State *state, double posX, double posY, string text, double colorR, double colorG, double colorB);
    string (*updateString)(State*); 

    void Draw();
};

class UIManager{
private:
    std::list<UIElement*> elements;
    int activeMenu;

    State *state;

public:
    UIManager(State* state);
    ~UIManager();
    
    void Draw();
    void AddUIElement(UIElement *element);
};