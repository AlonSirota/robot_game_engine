#include "ui.hpp"

TextLable::TextLable(State *state, double posX, double posY, string text, double colorR, double colorG, double colorB):UIElement(state, posX, posY){
    this->text.assign(text);
    this->color[0] = colorR;
    this->color[1] = colorG;
    this->color[2] = colorB;
    this->updateString = nullptr;
};


void TextLable::Draw(){
    if(!this->isVisible){return;};

    const char *lable1;
    if(this->updateString == nullptr){
        lable1 = this->text.c_str();
    }else{
        lable1 = this->updateString(this->state).c_str();
    }

    int pos = 0;
    glColor3f(this->color[0], this->color[1], this->color[2]);
    glRasterPos2d(this->posX, this->posY);
    while(lable1[pos]){//nullterminated string
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, lable1[pos++]);
    }
}

UIManager::UIManager(State *state){
    this->state = state;

    
    TextLable *controlModeLable = new TextLable(state, 10, 970, "", 1, 1, 1);
    controlModeLable->updateString = [](State* state) -> string {if(!state->displayDebugInfo){return "";} return "Control Mode = " + string((state->controlMode == Robot ? "Robot" : "Camera"));};
    this->AddUIElement(controlModeLable);

    TextLable *MovingForwardLable = new TextLable(state, 10, 940, "", 1, 1, 1);
    MovingForwardLable->updateString = [](State* state) -> string {if(!state->displayDebugInfo){return "";} return "isMovingForward = " + string((state->isMovingForward) ? "True" : "False");};
    this->AddUIElement(MovingForwardLable);

    TextLable *RotatingLeftLable = new TextLable(state, 10, 910, "", 1, 1, 1);
    RotatingLeftLable->updateString = [](State* state) -> string {if(!state->displayDebugInfo){return "";} return "isRotatingLeft = " + string((state->isRotatingLeft) ? "True" : "False");};
    this->AddUIElement(RotatingLeftLable);

    TextLable *RotatingRightLable = new TextLable(state, 10, 880, "", 1, 1, 1);
    RotatingRightLable->updateString = [](State* state) -> string {if(!state->displayDebugInfo){return "";} return "isRotatingRight = " + string((state->isRotatingRight) ? "True" : "False");};
    this->AddUIElement(RotatingRightLable);

    TextLable *RobotPositionLable = new TextLable(state, 10, 850, "", 1, 1, 1);
    RobotPositionLable->updateString = [](State* state) -> string {if(!state->displayDebugInfo){return "";} return "RobotPosition = x=" + std::to_string(state->robot.position.x) + " y=" + std::to_string(state->robot.position.y) + " z=" + std::to_string(state->robot.position.z);};
    this->AddUIElement(RobotPositionLable);

    TextLable *CameraPositionLable = new TextLable(state, 10, 820, "", 1, 1, 1);
    CameraPositionLable->updateString = [](State* state) -> string {if(!state->displayDebugInfo){return "";} return "CameraPosition = x=" + std::to_string(state->camera.position.x) + " y=" + std::to_string(state->camera.position.y) + " z=" + std::to_string(state->camera.position.z);};
    this->AddUIElement(CameraPositionLable);

}

void UIManager::AddUIElement(UIElement *element){
    this->elements.push_back(element);
}

void UIManager::Draw(){
    for (auto element : elements){
        element->Draw();
    };
};
UIManager::~UIManager(){
    for (auto element : this->elements){
        delete element;
    }
}