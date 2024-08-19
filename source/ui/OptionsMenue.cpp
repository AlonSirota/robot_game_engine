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

  TextBox *AmbiantLightRedText = new TextBox(state, 300, 700, "Enter Ambiant Light Red (0-255):", 1, 1, 1, 0, 0, 0, 2);
  this->AddUIElement(AmbiantLightRedText);
  
  TextBoxInput *AmbiantLightRed = new TextBoxInput(state, 1250, 700, "", 0, 0, 0, 1, 1, 1, 10, 2);
  AmbiantLightRed->update = [](UIElement* meOrg, State  *state) -> void {
    TextBoxInput *me = (TextBoxInput *)meOrg;
    state->AmbientR = atoi(me->actualText.c_str());
  };
  AmbiantLightRed->actualText = std::to_string(state->AmbientR);
  this->AddUIElement(AmbiantLightRed);

  TextBox *AmbiantLightGreenText = new TextBox(state, 300, 640, "Enter Ambiant Light Green (0-255):", 1, 1, 1, 0, 0, 0, 2);
  this->AddUIElement(AmbiantLightGreenText);
  
  TextBoxInput *AmbiantLightGreen = new TextBoxInput(state, 1250, 640, "", 0, 0, 0, 1, 1, 1, 10, 2);
  AmbiantLightGreen->update = [](UIElement* meOrg, State  *state) -> void {
    TextBoxInput *me = (TextBoxInput *)meOrg;
    state->AmbientG = atoi(me->actualText.c_str());
  };
  AmbiantLightGreen->actualText = std::to_string(state->AmbientG);
  this->AddUIElement(AmbiantLightGreen);

  TextBox *AmbiantLightBlueText = new TextBox(state, 300, 580, "Enter Ambiant Light Blue (0-255):", 1, 1, 1, 0, 0, 0, 2);
  this->AddUIElement(AmbiantLightBlueText);
  
  TextBoxInput *AmbiantLightBlue = new TextBoxInput(state, 1250, 580, "", 0, 0, 0, 1, 1, 1, 10, 2);
  AmbiantLightBlue->update = [](UIElement* meOrg, State  *state) -> void {
    TextBoxInput *me = (TextBoxInput *)meOrg;
    state->AmbientB = atoi(me->actualText.c_str());
  };
  AmbiantLightBlue->actualText = std::to_string(state->AmbientB);
  this->AddUIElement(AmbiantLightBlue);

  TextBox *AmbiantLightIText = new TextBox(state, 300, 520, "Enter Ambiant Light Intensity (0-255):", 1, 1, 1, 0, 0, 0, 2);
  this->AddUIElement(AmbiantLightIText);
  
  TextBoxInput *AmbiantLightI = new TextBoxInput(state, 1250, 520, "", 0, 0, 0, 1, 1, 1, 10, 2);
  AmbiantLightI->update = [](UIElement* meOrg, State  *state) -> void {
    TextBoxInput *me = (TextBoxInput *)meOrg;
    state->AmbientI = atoi(me->actualText.c_str());
  };
  AmbiantLightI->actualText = std::to_string(state->AmbientI);
  this->AddUIElement(AmbiantLightI);

  SolidColor *MenuBackground = new SolidColor(state, 0, 0, 1920, 1080, 0, 0, 0, 0.5);
  this->AddUIElement(MenuBackground);

}
