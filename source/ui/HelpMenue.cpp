#include "HelpMenue.hpp"

string controles[] = {
  "Uses W A S D for movement",
  "Use Esc key to open / close menue",
  "press 1 to control robot",
  "press 2 to control Camera"
};

HelpMenue::HelpMenue(State *state) : UIMenue(state){
  this->isActive = true;
  this->isVisible = true;

  TextBox *BackButton = new TextBox(state, 50, 950, "Back", 1, 1, 1, 0, 0, 0, 3);
  BackButton->OnClick = [](State *state, double x, double y) -> void {
    state->activeMenue = Main;
  };
  this->AddUIElement(BackButton);

  double Y = 800;

  for (auto line : controles){
    TextBox *Text = new TextBox(state, 400, Y, line, 1, 1, 1, 0, 0, 0, 3);
    this->AddUIElement(Text);

    Y -= 80;
  }

  SolidColor *MenuBackground = new SolidColor(state, 0, 0, 1920, 1080, 0, 0, 0, 0.5);
  this->AddUIElement(MenuBackground);

}
