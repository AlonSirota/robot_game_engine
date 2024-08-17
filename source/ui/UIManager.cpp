#include "UIManager.hpp"

UIManager::UIManager(State *state){
    this->state = state;

    
    TextBox *controlModeLable = new TextBox(state, 10, 1050, "", 1, 1, 1, 0, 0, 0);
    controlModeLable->updateString = [](State* state) -> string {if(!state->displayDebugInfo){return "";} return "Control Mode = " + string((state->controlMode == Robot ? "Robot" : "Camera"));};
    this->AddUIElement(controlModeLable);

  TextLable *MovingForwardLable = new TextLable(state, 10, 1020, "", 1, 1, 1);
  MovingForwardLable->updateString = [](State *state) -> string {
    if (!state->displayDebugInfo) {
      return "";
    }
    return "isMovingForward = " +
           string((state->controlCommands.isMovingForward) ? "True" : "False");
  };
  this->AddUIElement(MovingForwardLable);

  TextLable *RotatingLeftLable = new TextLable(state, 10, 990, "", 1, 1, 1);
  RotatingLeftLable->updateString = [](State *state) -> string {
    if (!state->displayDebugInfo) {
      return "";
    }
    return "isRotatingLeft = " +
           string((state->controlCommands.isRotatingLeft) ? "True" : "False");
  };
  this->AddUIElement(RotatingLeftLable);

  TextLable *RotatingRightLable = new TextLable(state, 10, 960, "", 1, 1, 1);
  RotatingRightLable->updateString = [](State *state) -> string {
    if (!state->displayDebugInfo) {
      return "";
    }
    return "isRotatingRight = " +
           string((state->controlCommands.isRotatingRight) ? "True" : "False");
  };
  this->AddUIElement(RotatingRightLable);

  TextLable *RobotPositionLable = new TextLable(state, 10, 930, "", 1, 1, 1);
  RobotPositionLable->updateString = [](State *state) -> string {
    if (!state->displayDebugInfo) {
      return "";
    }
    return "RobotPosition = x=" +
           std::to_string(state->robot.transform.position.x) +
           " y=" + std::to_string(state->robot.transform.position.y) +
           " z=" + std::to_string(state->robot.transform.position.z);
  };
  this->AddUIElement(RobotPositionLable);

  TextLable *CameraPositionLable = new TextLable(state, 10, 900, "", 1, 1, 1);
  CameraPositionLable->updateString = [](State *state) -> string {
    if (!state->displayDebugInfo) {
      return "";
    }
    return "CameraPosition = x=" + std::to_string(state->camera.position.x) +
           " y=" + std::to_string(state->camera.position.y) +
           " z=" + std::to_string(state->camera.position.z);
  };
  this->AddUIElement(CameraPositionLable);

  this->mainMenue = new MainMenue(this->state);
  this->helpMenue = new HelpMenue(this->state);
  this->optionsMenue = new OptionsMenue(this->state);
}

void UIManager::AddUIElement(UIElement *element) {
  this->elements.push_back(element);
}

void UIManager::Draw() {
  switch(state->activeMenue){
    case Main:
      this->mainMenue->Draw();
      break;
    case Help:
      this->helpMenue->Draw();
      break;
    case Options:
      this->optionsMenue->Draw();
      break;
    case None:
    default:
      break;
  }

  for (auto element : elements) {
    element->Draw();
  };

};

UIManager::~UIManager() {
  for (auto element : this->elements) {
    delete element;
  }
}
bool UIManager::MouseClick(double mX, double mY){
    switch(state->activeMenue){
    case Main:
      if(this->mainMenue->MouseClick(mX, mY)){
        return true;
      }
      break;
    case Help:
      if(this->helpMenue->MouseClick(mX, mY)){
        return true;
      }
      break;
    case Options:
      if(this->optionsMenue->MouseClick(mX, mY)){
        return true;
      }
      break;
    case None:
    default:
      break;
  }

  for(auto element : this->elements){
    if(element->MouseClick(mX, mY)){
        return true;
    }
  };

    return false;
}