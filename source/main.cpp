#include <GL/freeglut_std.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cstdio>

#define ROBOT_SPEED 1
#define ROTATION_SPEED 100

enum ControlMode { Robot, Camera };

struct Point {
  float x;
  float y;
  float z;

  Point operator+(const Point &other) {
    return {x + other.x, y + other.y, z + other.z};
  }

  Point operator+=(const Point &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }
};
struct Transform {
  Point position;
  Point orientation;

  Transform operator+(const Transform &other) {
    return {position + other.position, orientation + other.orientation};
  }

  Transform operator+=(const Transform &other) {
    position += other.position;
    orientation += other.orientation;
    return *this;
  }
};

void clearMatrices();
void setupCamera();
void displayFunc();
void displayRobot(Transform transform);
void drawAxis();
void keyboardFunc(unsigned char key, int x, int y);
void keyboardUpFunc(unsigned char key, int x, int y);
void idleFunc();

struct State {
  ControlMode controlMode;
  bool isMovingForward;
  bool isRotatingLeft;
  bool isRotatingRight;
  Transform robot;

  State() {
    controlMode = Robot;
    isMovingForward = false;
    robot = {{0, 0, 0}, {0, 0, 0}};
  }
};

float presentedTime = 0;
State state = State();

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
  displayRobot(state.robot);
  glFlush();
}

void clearMatrices() {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
}

void displayRobot(Transform transform) {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef(transform.position.x, transform.position.y,
               transform.position.z);
  glRotatef(transform.orientation.x, 1, 0, 0);
  glRotatef(transform.orientation.y, 0, 1, 0);
  glRotatef(transform.orientation.z, 0, 0, 1);
  glutSolidTetrahedron();
  glPopMatrix();
}

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
}

void updatedState(State &currentState, float deltaTime) {
  Transform controlTranform = {{0, 0, 0}, {0, 0, 0}};
  if (state.isMovingForward) {
    controlTranform.position.y += ROBOT_SPEED * deltaTime;
  }

  if (state.isRotatingLeft) {
    controlTranform.orientation.z += ROTATION_SPEED * deltaTime;
  }

  if (state.isRotatingRight) {
    controlTranform.orientation.z -= ROTATION_SPEED * deltaTime;
  }

  if (state.controlMode == Robot) {
    state.robot += controlTranform;
  }
}

void idleFunc() {
  float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
  float deltaTime = currentTime - presentedTime;

  updatedState(state, deltaTime);

  presentedTime = currentTime;
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
