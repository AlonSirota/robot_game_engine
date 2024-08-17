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

enum ControlMode { Robot, RobotHead, UpperArm, LowerArm, Hand, Camera };
enum PointOfView { FirstPerson, ThirdPerson };

struct Transform {
  Point position;
  Quaternion quaternion;

  Transform(Point position, Quaternion quaternion)
      : position(position), quaternion(quaternion) {}
};

struct Robot {
  Robot(Transform transform, Quaternion headRotation, Quaternion armRotation,
        Quaternion elbowRotation, Quaternion handRotation)
      : transform(transform), headRotation(headRotation),
        armRotation(armRotation), elbowRotation(elbowRotation),
        handRotation(handRotation) {}
  Transform transform;

  // This rotation is relative to the torso.
  Quaternion headRotation;

  // Relative to the left shoulder pivot point.
  Quaternion armRotation;

  // Relative to the elbow pivot point.
  Quaternion elbowRotation;

  // Relative to the hand pivot point.
  Quaternion handRotation;
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
  PointOfView pointOfView;
  ControlMode controlMode;
  ControlCommands controlCommands;
  struct Robot robot;
  Transform camera;
  int windowHeight;
  int windowWidth;

  bool displayDebugInfo;

  State()
      : pointOfView(ThirdPerson), controlMode(Robot), controlCommands(),
        robot({{0, 0, 0}, {1, 0, 0}}, Quaternion::identity(),
              Quaternion::identity(), Quaternion::identity(),
              Quaternion::identity()),
        camera({{5, 5, 5}, Quaternion(0.88, -0.325, 0.325, 0)}),
        displayDebugInfo(true) {}
};

inline void moveForwardAndBackwords(Transform &t,
                                    ControlCommands controlCommands,
                                    bool shouldInvertForwardDirection,
                                    double deltaTime) {
  Point forward = t.quaternion.getForwardVector() * (shouldInvertForwardDirection ? -1 : 1);

  if (controlCommands.isMovingForward) {
    t.position += forward * ROBOT_SPEED * deltaTime;
  } else if (controlCommands.isMovingBackward) {
    t.position -= forward * ROBOT_SPEED * deltaTime;
  }
}

inline void moveLeftAndRight(Transform &t,
                             const ControlCommands &controlCommands,
                             double deltaTime) {
  if (controlCommands.isMovingLeft) {
    t.position -= t.quaternion.getRightVector() * ROBOT_SPEED * deltaTime;
  } else if (controlCommands.isMovingRight) {
    t.position += t.quaternion.getRightVector() * ROBOT_SPEED * deltaTime;
  }
}

inline void moveUpAndDown(Transform &t, const ControlCommands &controlCommands,
                          double deltaTime) {
  if (controlCommands.isMovingUp) {
    t.position += t.quaternion.getUpVector() * ROBOT_SPEED * deltaTime;
  } else if (controlCommands.isMovingDown) {
    t.position -= t.quaternion.getUpVector() * ROBOT_SPEED * deltaTime;
  }
}

inline void rotateLeftAndRight(Quaternion &quaternion,
                               const ControlCommands &controlCommands,
                               double deltaTime, double rotationSpeed) {
  Point up = quaternion.getUpVector() * 1;
  if (controlCommands.isRotatingLeft) {
    Quaternion rotation(rotationSpeed * deltaTime, up);
    quaternion = (rotation.normalize() * quaternion.normalize()).normalize();
  }
  if (controlCommands.isRotatingRight) {
    Quaternion rotation(-rotationSpeed * deltaTime, up);
    quaternion = (rotation.normalize() * quaternion.normalize()).normalize();
  }
}

inline void rotateUpAndDown(Quaternion &quaternion,
                            const ControlCommands &controlCommands,
                            double deltaTime, double rotationSpeed) {
  Point right = quaternion.getRightVector();
  if (controlCommands.isRotatingUp) {
    Quaternion rotation(rotationSpeed * deltaTime, right);
    quaternion = (rotation.normalize() * quaternion.normalize()).normalize();
  }
  if (controlCommands.isRotatingDown) {
    Quaternion rotation(-rotationSpeed * deltaTime, right);
    quaternion = (rotation.normalize() * quaternion.normalize()).normalize();
  }
}

inline void updatedState(State &currentState, double deltaTime) {
  switch (currentState.controlMode) {
  case Robot:
    moveForwardAndBackwords(currentState.robot.transform,
                            currentState.controlCommands, false, deltaTime);
    moveLeftAndRight(currentState.robot.transform, currentState.controlCommands,
                     deltaTime);
    rotateLeftAndRight(currentState.robot.transform.quaternion,
                       currentState.controlCommands, deltaTime,
                       ROBOT_ROTATION_SPEED);
    break;
  case RobotHead:
    rotateLeftAndRight(currentState.robot.headRotation,
                       currentState.controlCommands, deltaTime,
                       ROBOT_ROTATION_SPEED);
    break;
  case Camera:
    moveForwardAndBackwords(currentState.camera, currentState.controlCommands,
                            true, deltaTime);
    moveLeftAndRight(currentState.camera, currentState.controlCommands,
                     deltaTime);
    rotateUpAndDown(currentState.camera.quaternion,
                    currentState.controlCommands, deltaTime,
                    CAMERA_ROTATION_SPEED);
    rotateLeftAndRight(currentState.camera.quaternion,
                       currentState.controlCommands, deltaTime,
                       CAMERA_ROTATION_SPEED);
    moveUpAndDown(currentState.camera, currentState.controlCommands, deltaTime);
    break;
  case UpperArm:
    rotateUpAndDown(currentState.robot.armRotation,
                    currentState.controlCommands, deltaTime,
                    CAMERA_ROTATION_SPEED);
    break;
  case LowerArm:
    rotateUpAndDown(currentState.robot.elbowRotation,
                    currentState.controlCommands, deltaTime,
                    CAMERA_ROTATION_SPEED);
    break;
  case Hand:
    rotateLeftAndRight(currentState.robot.handRotation,
                       currentState.controlCommands, deltaTime,
                       CAMERA_ROTATION_SPEED);
    break;
  }
  currentState.windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
  currentState.windowWidth = glutGet(GLUT_WINDOW_WIDTH);
}
#endif
