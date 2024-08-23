#include "TextLable.hpp"

TextLable::TextLable(State *state, double posX, double posY, string text,
                     double colorR, double colorG, double colorB, double textSize)
    : UIElement(state, posX, posY) {
  this->text.assign(text);
  this->color[0] = colorR;
  this->color[1] = colorG;
  this->color[2] = colorB;
  this->textSize = textSize;
  this->updateString = nullptr;
};

void TextLable::Draw() {
  if (!this->isVisible) {
    return;
  };

  UIElement::Draw();

  if (this->updateString != nullptr) {
    this->text.assign(this->updateString(this->state));
  }

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  int pos = 0;
  glColor3f(this->color[0], this->color[1], this->color[2]);
  glTranslated(this->posX, this->posY, 0);
  double scaleFactor =
      RENDER_WIDTH / (glutStrokeWidthf(GLUT_STROKE_ROMAN, 'a') * 150);
  scaleFactor *= this->textSize;
  glScaled(scaleFactor, scaleFactor, scaleFactor);
  for(char c : this->text){
    glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
  }
  glPopMatrix();
}

void TextLable::calculateDimentions(){
        //hard coded values for GLUT_STROKE_ROMAN, couldnt find them in a define tho....
    double top = 119.05;
    double bottom = -33.33;
    double width = glutStrokeLength(GLUT_STROKE_ROMAN, (const unsigned char *) this->text.c_str());
    double scaleFactor = RENDER_WIDTH / (glutStrokeWidthf(GLUT_STROKE_ROMAN, 'a') * 150);

    scaleFactor *= this->textSize;

    top *= scaleFactor;
    bottom *= scaleFactor;
    width *= scaleFactor;

    this->minX = this->posX;
    this->minY = this->posY + bottom;

    this->maxX = minX + width;
    this->maxY = this->posY + top;
}

bool TextLable::MouseClick(double mX, double mY){
    this->calculateDimentions();
    bool isInside = mX >= this->minX && mX <= this->maxX && mY >= this->minY && mY <= this->maxY;
    if(isInside && this->OnClick != nullptr){
        this->OnClick(this->state, mX, mY);
    };

    return isInside;
}
