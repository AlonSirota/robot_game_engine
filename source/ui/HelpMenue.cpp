#include "HelpMenue.hpp"

string controles[] = {
  "Uses W A S D for movement",
  "Uses Arrow keys to look around",
  "Use Esc key to open / close menue",
  "Press the space bar to toggle 1st/3rd person view",
  "press 1 to control Camera",
  "press 2 to control Robot",
  "press 3 to control the Robots head",
  "press 4 to control the Robots Upper arm",
  "press 5 to control the Robots lower arm",
  "press 6 to control the Robots hand",
  "You can also use the mouse to select control point",
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
    TextBox *Text = new TextBox(state, 300, Y, line, 1, 1, 1, 0, 0, 0, 2);
    this->AddUIElement(Text);

    Y -= 55;
  }

  SolidColor *MenuBackground = new SolidColor(state, 0, 0, 1920, 1080, 0, 0, 0, 0.5);
  this->AddUIElement(MenuBackground);

}
