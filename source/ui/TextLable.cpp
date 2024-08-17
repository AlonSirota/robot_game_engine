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

  const char *lable1;
  if (this->updateString == nullptr) {
    lable1 = this->text.c_str();
  } else {
    lable1 = this->updateString(this->state).c_str();
        this->text.assign(lable1);
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
  while (lable1[pos]) { // nullterminated string
    glutStrokeCharacter(GLUT_STROKE_ROMAN, lable1[pos++]);
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
