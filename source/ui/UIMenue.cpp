#include "UIMenue.hpp"

UIMenue::UIMenue(State *state) : UIElement(state, 0, 0){
   
}

void UIMenue::Draw(){
    if(!this->isVisible){return;};

    for (auto element : elements){
        element->Draw();
    };
};

UIMenue::~UIMenue(){
    for (auto element : this->elements){
        delete element;
    }
}

bool UIMenue::MouseClick(double mX, double mY){
    if(!this->isActive){return false;};

    bool result = false;
    for(auto element : this->elements){
        if(element->MouseClick(mX, mY)){
            result = true;
        }
    };

    return result;
}
void UIMenue::AddUIElement(UIElement *element) {
  this->elements.push_back(element);
}

void UIMenue::KeyPress(unsigned char c){
    for(auto element : this->elements){
        element->KeyPress(c);
    };
}
