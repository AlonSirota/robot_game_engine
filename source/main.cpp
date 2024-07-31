#include <GL/freeglut_std.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <cmath>
#include <iostream>

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

struct Quaternion {
  // Member variables
  double w, x, y, z;

  // Constructor
  Quaternion(double w = 1.0, double x = 0.0, double y = 0.0, double z = 0.0)
      : w(w), x(x), y(y), z(z) {}

  Quaternion(double angle, Point axis) {
    w = std::cos(angle / 2);
    x = axis.x * std::sin(angle / 2);
    y = axis.y * std::sin(angle / 2);
    z = axis.z * std::sin(angle / 2);
  }

  // Quaternion multiplication
  Quaternion operator*(const Quaternion &q) const {
    return Quaternion(w * q.w - x * q.x - y * q.y - z * q.z,
                      w * q.x + x * q.w + y * q.z - z * q.y,
                      w * q.y - x * q.z + y * q.w + z * q.x,
                      w * q.z + x * q.y - y * q.x + z * q.w);
  }

  // Scalar multiplication
  Quaternion operator*(double scalar) const {
    return Quaternion(w * scalar, x * scalar, y * scalar, z * scalar);
  }

  // Scalar division
  Quaternion operator/(double scalar) const {
    return Quaternion(w / scalar, x / scalar, y / scalar, z / scalar);
  }

  // Conjugate of the quaternion
  Quaternion conjugate() const { return Quaternion(w, -x, -y, -z); }

  // Magnitude (norm) of the quaternion
  double magnitude() const { return std::sqrt(w * w + x * x + y * y + z * z); }

  // Normalization of the quaternion
  Quaternion normalize() const {
    double mag = magnitude();
    return Quaternion(w / mag, x / mag, y / mag, z / mag);
  }

  // Inverse of the quaternion
  Quaternion inverse() const {
    return conjugate() / (magnitude() * magnitude());
  }

  Point rotatePoint(const Point &point) const {
    Quaternion p(0, point.x, point.y, point.z);
    Quaternion q = (*this) * p * inverse();
    return {q.x, q.y, q.z};
  }

  // Display the quaternion
  void display() const {
    std::cout << "Quaternion: (" << w << ", " << x << ", " << y << ", " << z
              << ")\n";
  }
};
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

struct State {
  ControlMode controlMode;
  bool isMovingForward;
  bool isRotatingLeft;
  bool isRotatingRight;
  Transform robot;
  Transform camera;

  State()
      : controlMode(Robot), isMovingForward(false), isRotatingLeft(false),
        isRotatingRight(false), robot({{0, 0, 0}, {1, 0, 1}}),
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

void displayRobot(Transform robotTransform) {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef(robotTransform.position.x, robotTransform.position.y,
               robotTransform.position.z);
  // glRotatef(transform.orientation.x, 1, 0, 0);
  // glRotatef(transform.orientation.y, 0, 1, 0);
  // glRotatef(transform.orientation.z, 0, 0, 1);
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
