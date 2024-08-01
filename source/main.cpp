#include <GL/freeglut_std.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "Quaternion.h"

#define ROBOT_SPEED 1
#define ROBOT_ROTATION_SPEED 3
#define CAMERA_ROTATION_SPEED 1

#define TARGET_FRAME_RATE 60
#define TARGET_DELTA_BETWEEN_FRAMES_MS (1000.0 / TARGET_FRAME_RATE);

enum ControlMode { Robot, Camera };

struct Transform {
  Point position;
  Quaternion quaternion;

  Transform(Point position, Quaternion quaternion)
      : position(position), quaternion(quaternion) {}
};

void clearMatrices();
void setupCamera(Transform camera);
void displayFunc();
void displayRobot(Transform transform);
void drawAxis();
void keyboardFunc(unsigned char key, int x, int y);
void keyboardUpFunc(unsigned char key, int x, int y);
void idleFunc();
void PostRedisplayWrapper(int);

struct State {
  ControlMode controlMode;
  bool isMovingForward;
  bool isRotatingLeft;
  bool isRotatingRight;
  Transform robot;
  Transform camera;

  State()
      : controlMode(Robot), isMovingForward(false), isRotatingLeft(false),
        isRotatingRight(false), robot({{0, 0, 0}, {1, 0, 0}}),
        camera({{5,5,5}, Quaternion(0.88, -0.325, 0.325,0)}) {}
};

double presentedTime = 0;
State state = State();

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

void displayRobot(Transform robotTransform) {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef(robotTransform.position.x, robotTransform.position.y,
               robotTransform.position.z);
  glMultMatrixf(robotTransform.quaternion.toMatrix());
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
}

void move(Transform &t, bool isMovingForward, bool isRotatingLeft,
          bool isRotatingRight, double deltaTime, double rotationSpeed) {
  if (isMovingForward) {
    t.position +=
        t.quaternion.rotatePoint(Point(0, 0, 1)) * ROBOT_SPEED * deltaTime;
  }

  if (isRotatingLeft) {
    t.quaternion =
        t.quaternion * Quaternion(rotationSpeed * deltaTime, Point(0, 1, 0));
  }

  if (isRotatingRight) {
    t.quaternion =
        t.quaternion * Quaternion(-rotationSpeed * deltaTime, Point(0, 1, 0));
  }
}

void updatedState(State &currentState, double deltaTime) {
  switch (state.controlMode) {
  case Robot:
    move(state.robot, state.isMovingForward, state.isRotatingLeft,
         state.isRotatingRight, deltaTime, ROBOT_ROTATION_SPEED);
    break;
  case Camera:
    move(state.camera, state.isMovingForward, state.isRotatingLeft,
         state.isRotatingRight, deltaTime, CAMERA_ROTATION_SPEED);
    break;
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
