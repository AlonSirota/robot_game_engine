#include "HelpMenue.hpp"

const char* controles[] = {
  "Use W A S D for movement",
  "Use Q for up movement",
  "Use E for down movement",
  "Use Arrow keys to look around",
  "Use Esc key to open / close menue",
  "Press the space bar to toggle 1st/3rd person view",
  "Ese the mouse to select control point",
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

  for (const char *line : controles){
    TextBox *Text = new TextBox(state, 300, Y, string(line), 1, 1, 1, 0, 0, 0, 2);
    this->AddUIElement(Text);

    Y -= 55;
  }

  SolidColor *MenuBackground = new SolidColor(state, 0, 0, 1920, 1080, 0, 0, 0, 0.5);
  this->AddUIElement(MenuBackground);

}
