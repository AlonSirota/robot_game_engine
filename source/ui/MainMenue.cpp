#include "MainMenue.hpp"

MainMenue::MainMenue(State *state) : UIMenue(state){
  this->isActive = true;
  this->isVisible = true;

  TextBox *BackButton = new TextBox(state, 50, 950, "Back", 1, 1, 1, 0, 0, 0, 3);
    BackButton->OnClick = [](State *state, double x, double y) -> void {
        state->activeMenue = None;
    };
    this->AddUIElement(BackButton);
    
  TextBox *QuitLable = new TextBox(state, 800, 400, "Quit", 1, 1, 1, 0, 0, 0, 5);
  QuitLable->OnClick = [](State *state, double x, double y) -> void {
    exit(0);
  };
  this->AddUIElement(QuitLable);

  TextBox *HelpLable = new TextBox(state, 800, 600, "Help", 1, 1, 1, 0, 0, 0, 5);
  HelpLable->OnClick = [](State *state, double x, double y) -> void {
    state->activeMenue = Help;
  };
  this->AddUIElement(HelpLable);

  TextBox *OptionsLable = new TextBox(state, 800, 800, "Options", 1, 1, 1, 0, 0, 0, 5);
  OptionsLable->OnClick = [](State *state, double x, double y) -> void {
    state->activeMenue = Options;
  };
  this->AddUIElement(OptionsLable);

  SolidColor *MenuBackground = new SolidColor(state, 0, 0, 1920, 1080, 0, 0, 0, 0.5);
  this->AddUIElement(MenuBackground);
}
