#ifndef STATE_H
#define STATE_H
#include "Quaternion.h"

#define ROBOT_SPEED 1
#define ROBOT_ROTATION_SPEED 1
#define CAMERA_ROTATION_SPEED 1

//Render resolution of 1920x1080 (standard Full HD)
#define RENDER_WIDTH 1920
#define RENDER_HIGHT 1080
#define RENDER_ASPECT_RATIO ((float)RENDER_WIDTH / (float)RENDER_HIGHT)

enum ControlMode { Robot, Camera };

struct Transform {
  Point position;
  Quaternion quaternion;

  Transform(Point position, Quaternion quaternion)
      : position(position), quaternion(quaternion) {}

  Point direction() { return quaternion.rotatePoint({0, 0, 1}); }
};

struct Robot {
  Robot(Transform transform, Quaternion headRotation)
      : transform(transform), headRotation(headRotation) {}
  Transform transform;
  // This rotation is relative to the torso.
  Quaternion headRotation;
};

struct State {
  ControlMode controlMode;
  bool isMovingForward;
  bool isRotatingLeft;
  bool isRotatingRight;
  bool isMovingBackward;
  struct Robot robot;
  Transform camera;
  int windowHeight;
  int windowWidth;


  bool displayDebugInfo;

  State()
      : controlMode(Robot), isMovingForward(false), isRotatingLeft(false),
        isRotatingRight(false), robot({{0, 0, 0}, {1, 0, 0}}, Quaternion::identity()),
        camera({{5, 5, 5}, Quaternion(0.88, -0.325, 0.325, 0)}),
        displayDebugInfo(true) {}
};

inline void move(Transform &t, bool isMovingForward, bool isMovingBackward,
                 bool isRotatingLeft, bool isRotatingRight, double deltaTime,
                 double rotationSpeed) {
  if (isMovingForward) {
    t.position += t.direction() * ROBOT_SPEED * deltaTime;
  } else if (isMovingBackward) {
    t.position -= t.direction() * ROBOT_SPEED * deltaTime;
  }

  if (isRotatingLeft) {
    Quaternion rotation(-rotationSpeed * deltaTime, Point(0, -1, 0));
    t.quaternion = (t.quaternion * rotation).normalize();
  }

  if (isRotatingRight) {
    Quaternion rotation(rotationSpeed * deltaTime, Point(0, -1, 0));
    t.quaternion = (t.quaternion * rotation).normalize();
  }
}

inline void updatedState(State &currentState, double deltaTime) {
  switch (currentState.controlMode) {
  case Robot:
    move(currentState.robot.transform, currentState.isMovingForward,
         currentState.isMovingBackward, currentState.isRotatingLeft,
         currentState.isRotatingRight, deltaTime, ROBOT_ROTATION_SPEED);
    break;
  case Camera:
    move(currentState.camera, currentState.isMovingForward,
         currentState.isMovingBackward, currentState.isRotatingLeft,
         currentState.isRotatingRight, deltaTime, CAMERA_ROTATION_SPEED);
    break;
  }
  
  currentState.windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
  currentState.windowWidth = glutGet(GLUT_WINDOW_WIDTH);
}
#endif
