#include <GL/freeglut_std.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cstdio>

#define ROBOT_SPEED 1

void clearMatrices();
void setupCamera();
void displayFunc();
void displayHead();
void drawAxis();
void keyboardFunc(unsigned char key, int x, int y);
void keyboardUpFunc(unsigned char key, int x, int y);
void idleFunc();

struct Point {
  float x;
  float y;
  float z;
};

struct State {
  bool isMovingForward;
  float presentedTime;
  Point robotPosition;

  State(bool isMovingForward, float presentedTime, Point robotPosition)
      : isMovingForward(isMovingForward), presentedTime(presentedTime),
        robotPosition(robotPosition) {}
};

static State state = State(false, 0, Point{0, 0, 0});

void drawAxis() {
  glBegin(GL_LINES);
  // y-axis (green)
  glColor3f(0, 1, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 5, 0);

  // x-axis (red)
  glColor3f(1, 0, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(5, 0, 0);

  // z-axis (blue)
  glColor3f(0, 0, 1);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, 5);
  glEnd();
}

void setupProjection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, 1, 1, 100);
}

void setupCamera() {
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(0, 0, 10, 0, 0, 1, 0, 1, 0);
}

void displayFunc() {
  glClear(GL_COLOR_BUFFER_BIT);
  clearMatrices();
  setupCamera();
  setupProjection();
  drawAxis();
  displayHead();
  glFlush();
}

void clearMatrices() {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
}

void displayHead() {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef(state.robotPosition.x, state.robotPosition.y, state.robotPosition.z);
  glutSolidTetrahedron();
  glPopMatrix();
}

void keyboardFunc(unsigned char key, int x, int y) {
  if (key == 'w') {
    state.isMovingForward = true;
  }
}

void keyboardUpFunc(unsigned char key, int x, int y) {
  if (key == 'w') {
    state.isMovingForward = false;
  }
}

void idleFunc() {
  float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
  float deltaTime = currentTime - state.presentedTime;

  if (state.isMovingForward) {
    state.robotPosition.y += ROBOT_SPEED * deltaTime;
  }

  state.presentedTime = currentTime;
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
