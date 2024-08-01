#ifndef STATE_H
#define STATE_H
#include "Quaternion.h"

#define ROBOT_SPEED 1
#define ROBOT_ROTATION_SPEED 3
#define CAMERA_ROTATION_SPEED 1

enum ControlMode { Robot, Camera };

struct Transform {
  Point position;
  Quaternion quaternion;

  Transform(Point position, Quaternion quaternion)
      : position(position), quaternion(quaternion) {}
};

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
  switch (currentState.controlMode) {
  case Robot:
    move(currentState.robot, currentState.isMovingForward, currentState.isRotatingLeft,
         currentState.isRotatingRight, deltaTime, ROBOT_ROTATION_SPEED);
    break;
  case Camera:
    move(currentState.camera, currentState.isMovingForward, currentState.isRotatingLeft,
         currentState.isRotatingRight, deltaTime, CAMERA_ROTATION_SPEED);
    break;
  }
}
#endif
