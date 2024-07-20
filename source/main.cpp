#include <GL/freeglut_std.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <cmath>
#include <cstdio>

#define ROBOT_SPEED 1
#define ROBOT_ROTATION_SPEED 10
#define CAMERA_ROTATION_SPEED 1

enum ControlMode { Robot, Camera };

struct Point {
  Point(double x, double y, double z) : x(x), y(y), z(z) {}
  double x;
  double y;
  double z;

  Point operator+(const Point &other) {
    return {x + other.x, y + other.y, z + other.z};
  }

  Point operator+=(const Point &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  Point operator-(const Point &other) {
    return {x - other.x, y - other.y, z - other.z};
  }

  Point operator*(double scalar) {
    return {x * scalar, y * scalar, z * scalar};
  }
};
struct Transform {
  Point position;
  Point orientation;

  Transform(Point position, Point orientation)
      : position(position), orientation(orientation) {}

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
void setupCamera(Transform camera);
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
  Transform camera;

  State()
      : controlMode(Robot), isMovingForward(false), isRotatingLeft(false),
        isRotatingRight(false), robot({{0, 0, 0}, {1,0,1}}),
        camera({{5, 5, 5}, {-5, -5, -5}}) {}
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
  gluLookAt(camera.position, camera.position + camera.orientation,
            Point(0, 1, 0));
}

void displayFunc() {
  glClear(GL_COLOR_BUFFER_BIT);
  clearMatrices();
  setupCamera(state.camera);
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
  // glRotatef(transform.orientation.x, 1, 0, 0);
  // glRotatef(transform.orientation.y, 0, 1, 0);
  // glRotatef(transform.orientation.z, 0, 0, 1);
  glRotated(1, transform.orientation.x, transform.orientation.y, transform.orientation.z);
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

Point rotateAroundAxis(Point point, Point axis, double angle) {
    double cosA = cos(angle);
    double sinA = sin(angle);
    double oneMinusCosA = 1 - cosA;

    double x = point.x, y = point.y, z = point.z;
    double u = axis.x, v = axis.y, w = axis.z;

    double newX = u*(u*x + v*y + w*z)*(oneMinusCosA) + x*cosA + (-w*y + v*z)*sinA;
    double newY = v*(u*x + v*y + w*z)*(oneMinusCosA) + y*cosA + (w*x - u*z)*sinA;
    double newZ = w*(u*x + v*y + w*z)*(oneMinusCosA) + z*cosA + (-v*x + u*y)*sinA;

    return {newX, newY, newZ};
}

Point rotatedAroundYAxis(Point point, double angle) {
  // return {point.x * cos(angle) + point.z * sin(angle), point.y,
  //         -point.x * sin(angle) + point.z * cos(angle)};
  return rotateAroundAxis(point, {1, 0, 0}, angle);
}

void move(Transform &t, bool isMovingForward, bool isRotatingLeft,
          bool isRotatingRight, double deltaTime, double rotationSpeed) {
  if (isMovingForward) {
    t.position += t.orientation * ROBOT_SPEED * deltaTime;
  }

  if (isMovingForward) {
    t.position += t.orientation * ROBOT_SPEED * deltaTime;
  }

  if (isRotatingLeft) {
    t.orientation =
        rotatedAroundYAxis(t.orientation, rotationSpeed * deltaTime);
  }

  if (isRotatingRight) {
    t.orientation =
        rotatedAroundYAxis(t.orientation, -rotationSpeed * deltaTime);
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
