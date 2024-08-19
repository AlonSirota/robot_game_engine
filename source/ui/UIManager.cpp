#include "UIManager.hpp"

UIManager::UIManager(State *state){
  this->state = state;

  for(auto element : AddDebugInfo(state)){
    this->AddUIElement(element);
  }

  for(auto element : AddControlInfo(state)){
    this->AddUIElement(element);
  }

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