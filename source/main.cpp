#include <GL/freeglut_std.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "Quaternion.h"
#include "State.h"
#include "ui/ui.hpp"


#define TARGET_FRAME_RATE 60
#define TARGET_DELTA_BETWEEN_FRAMES_MS (1000.0 / TARGET_FRAME_RATE);

void clearMatrices();
void pushMatrices();
void popMatrices();
void setupCamera(Transform camera);
void displayFunc();
void displayRobot(Transform transform);
void drawAxis();
void keyboardFunc(unsigned char key, int x, int y);
void keyboardUpFunc(unsigned char key, int x, int y);
void idleFunc();
void PostRedisplayWrapper(int);
void displayUI();

double presentedTime = 0;
State state = State();

UIManager uiManager(&state);

void drawAxis() {
  glBegin(GL_LINES);
  // y-axis (green)
  glColor3f(0, 1, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 100, 0);

  // x-axis (red)
  glColor3f(1, 0, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(100, 0, 0);

  // z-axis (blue)
  glColor3f(0, 0, 1);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, 100);
  glEnd();
}

void setupProjection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, 1, 1, 100);
}

void gluLookAt(Point camera, Point target, Point up) {
  gluLookAt(camera.x, camera.y, camera.z, target.x, target.y, target.z, up.x,
            up.y, up.z);
}

void setupCamera(Transform camera) {
  glMatrixMode(GL_MODELVIEW);
  Point direction = camera.quaternion.rotatePoint({0, 0, -1});
  Point target = camera.position + direction;
  Point up = camera.quaternion.rotatePoint({0, 1, 0});
  gluLookAt(camera.position, target, up);
}

void displayFunc() {
  double frameStartTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;

  glClear(GL_COLOR_BUFFER_BIT);
  clearMatrices();
  setupCamera(state.camera);
  setupProjection();
  drawAxis();
  displayRobot(state.robot);
  displayUI();

  glFlush();

  double frameEndTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
  double delta = frameEndTime - frameStartTime;
  double timeToNextFrame = TARGET_DELTA_BETWEEN_FRAMES_MS - delta;

  if(timeToNextFrame <= 0){
    timeToNextFrame = 0;//TODO: mark frame skipped, add to log
  }

  glutTimerFunc(timeToNextFrame, PostRedisplayWrapper, 0);
}

void clearMatrices() {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
}

void pushMatrices(){
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
}

void popMatrices(){
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
}

void glVertexPoint(Point p) { glVertex3f(p.x, p.y, p.z); }

void glTranslatePoint(Point p) { glTranslatef(p.x, p.y, p.z); }

void displayMovingDirection(Transform t) {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glBegin(GL_LINES);
  glColor3f(1, 0, 0);
  glVertexPoint(t.position);
  glVertexPoint(t.position + t.direction() * 2);
  glEnd();
  glPopMatrix();
}

void displayRobot(Transform robotTransform) {
  displayMovingDirection(robotTransform);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatePoint(robotTransform.position);
  glMultMatrixf(robotTransform.quaternion.toMatrix());
  glutSolidTetrahedron();
  glPopMatrix();
}

void displayUI(){
  pushMatrices();
  clearMatrices();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0,1000,0,1000);

  uiManager.Draw();

  popMatrices();
};

void keyboardFunc(unsigned char key, int x, int y) {
  if (key == 'w') {
    state.isMovingForward = true;
  }

  if (key == 'a') {
    state.isRotatingLeft = true;
  }

  if (key == 'd') {
    state.isRotatingRight = true;
  }

  if (key == 's') {
    state.isMovingBackward = true;
  }

  if (key == ' ') {
    state.controlMode = state.controlMode == Robot ? Camera : Robot;
  }
}

void keyboardUpFunc(unsigned char key, int x, int y) {
  if (key == 'w') {
    state.isMovingForward = false;
  }

  if (key == 'a') {
    state.isRotatingLeft = false;
  }

  if (key == 'd') {
    state.isRotatingRight = false;
  }

  if (key == 's') {
    state.isMovingBackward = false;
  }
}

void idleFunc() {
  double currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
  double deltaTime = currentTime - presentedTime;

  updatedState(state, deltaTime);

  presentedTime = currentTime;
}

void PostRedisplayWrapper(int){
  glutPostRedisplay();
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitWindowSize(1000, 1000);
  glutCreateWindow("Final project.");

  glutDisplayFunc(displayFunc);
  glutIdleFunc(idleFunc);
  glutKeyboardFunc(keyboardFunc);
  glutKeyboardUpFunc(keyboardUpFunc);
  
  glutMainLoop();
  return 0;
}
