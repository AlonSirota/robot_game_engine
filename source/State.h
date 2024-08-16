#ifndef STATE_H
#define STATE_H
#include "Quaternion.h"

#define ROBOT_SPEED 1
#define ROBOT_ROTATION_SPEED 1
#define CAMERA_ROTATION_SPEED 1

// Render resolution of 1920x1080 (standard Full HD)
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
  Point right() { return quaternion.rotatePoint({1, 0, 0}); }
  Point up() { return quaternion.rotatePoint({0, 1, 0}); }
};

struct Robot {
  Robot(Transform transform, Quaternion headRotation)
      : transform(transform), headRotation(headRotation) {}
  Transform transform;
  // This rotation is relative to the torso.
  Quaternion headRotation;
};

struct ControlCommands {
  bool isMovingForward;
  bool isMovingBackward;
  bool isRotatingLeft;
  bool isRotatingRight;
  bool isRotatingUp;
  bool isRotatingDown;
  bool isMovingLeft;
  bool isMovingRight;
  bool isMovingUp;
  bool isMovingDown;

  ControlCommands()
      : isMovingForward(false), isMovingBackward(false), isRotatingLeft(false),
        isRotatingRight(false), isMovingLeft(false), isMovingRight(false),
        isMovingUp(false), isMovingDown(false) {}
};

struct State {
  ControlMode controlMode;
  ControlCommands controlCommands;
  struct Robot robot;
  Transform camera;
  int windowHeight;
  int windowWidth;

  bool displayDebugInfo;

  State()
      : controlMode(Robot), controlCommands(),
        robot({{0, 0, 0}, {1, 0, 0}}, Quaternion::identity()),
        camera({{5, 5, 5}, Quaternion(0.88, -0.325, 0.325, 0)}),
        displayDebugInfo(true) {}
};

inline void move(Transform &t, ControlCommands controlCommands,
                 bool shouldInvertForwardDirection,
                 bool shouldEnableUpDownRotations, double deltaTime,
                 double rotationSpeed) {
  Point forward = t.direction() * (shouldInvertForwardDirection ? -1 : 1);

  if (controlCommands.isMovingForward) {
    t.position += forward * ROBOT_SPEED * deltaTime;
  } else if (controlCommands.isMovingBackward) {
    t.position -= forward * ROBOT_SPEED * deltaTime;
  }

  if (controlCommands.isMovingLeft) {
    t.position -= t.right() * ROBOT_SPEED * deltaTime;
  } else if (controlCommands.isMovingRight) {
    t.position += t.right() * ROBOT_SPEED * deltaTime;
  }

  if (controlCommands.isMovingUp) {
    t.position += t.up() * ROBOT_SPEED * deltaTime;
  } else if (controlCommands.isMovingDown) {
    t.position -= t.up() * ROBOT_SPEED * deltaTime;
  }

  if (controlCommands.isRotatingLeft) {
    Quaternion rotation(-rotationSpeed * deltaTime, Point(0, -1, 0));
    t.quaternion = (t.quaternion * rotation).normalize();
  }

  if (controlCommands.isRotatingRight) {
    Quaternion rotation(rotationSpeed * deltaTime, Point(0, -1, 0));
    t.quaternion = (t.quaternion * rotation).normalize();
  }

  if (shouldEnableUpDownRotations) {
    if (controlCommands.isRotatingUp) {
      Quaternion rotation(rotationSpeed * deltaTime, Point(1, 0, 0));
      t.quaternion = (t.quaternion * rotation).normalize();
    }
    if (controlCommands.isRotatingDown) {
      Quaternion rotation(-rotationSpeed * deltaTime, Point(1, 0, 0));
      t.quaternion = (t.quaternion * rotation).normalize();
    }
  }
}

inline void updatedState(State &currentState, double deltaTime) {
  switch (currentState.controlMode) {
  case Robot:
    move(currentState.robot.transform, currentState.controlCommands, false,
         false, deltaTime, ROBOT_ROTATION_SPEED);
    break;
  case Camera:
    move(currentState.camera, currentState.controlCommands, true, true,
         deltaTime, CAMERA_ROTATION_SPEED);
    break;
  }

  currentState.windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
  currentState.windowWidth = glutGet(GLUT_WINDOW_WIDTH);
}
#endif
