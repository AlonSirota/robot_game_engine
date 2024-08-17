#include "OptionsMenue.hpp"

OptionsMenue::OptionsMenue(State *state) : UIMenue(state){
  this->isActive = true;
  this->isVisible = true;

  TextBox *BackButton = new TextBox(state, 50, 950, "Back", 1, 1, 1, 0, 0, 0, 3);
  BackButton->OnClick = [](State *state, double x, double y) -> void {
      state->activeMenue = Main;
  };
  this->AddUIElement(BackButton);

    //"Enable debug Into :"
  TextBox *DebugEnable = new TextBox(state, 400, 800, "", 1, 1, 1, 0, 0, 0, 3);
  DebugEnable->OnClick = [](State *state, double x, double y) -> void {
    state->displayDebugInfo = !state->displayDebugInfo;
  };
  DebugEnable->updateString = [](State *state) -> string {
    return "Enable debug Into : " +
           string((state->displayDebugInfo) ? "True" : "False");
  };
  this->AddUIElement(DebugEnable);

  SolidColor *MenuBackground = new SolidColor(state, 0, 0, 1920, 1080, 0, 0, 0, 0.5);
  this->AddUIElement(MenuBackground);

}
