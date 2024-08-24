#ifndef STATE_H
#define STATE_H
#include "Point.h"
#include "Quaternion.h"

#define MOVE_SPEED 5
#define ROBOT_ROTATION_SPEED 1
#define CAMERA_ROTATION_SPEED 1

// Render resolution of 1920x1080 (standard Full HD)
#define RENDER_WIDTH 1920
#define RENDER_HIGHT 1080
#define RENDER_ASPECT_RATIO ((float)RENDER_WIDTH / (float)RENDER_HIGHT)

// The object that is currenlty being controlled, for example, if ControlMode is
// Hand, pressing the arrow keys will rotate the Hand.
enum ControlMode { Robot, RobotHead, UpperArm, LowerArm, Hand, Camera, Lamp };

/// The point of view of the camera, when FirstPerson, the camera is positioned
/// at the head of the robot, and in third person the camera is positioned
/// according the the Camera position in the state.
enum PointOfView { FirstPerson, ThirdPerson };

// The current menu shown to the user.
enum ActiveMenue {
  // No menu, the scene is interactable with the keyboard.
  None,

  // The main UI menu, allowing navigations to the rest of the menus.
  Main,

  // The help screen, showing instructions on how to interact with the program.
  Help,

  // Options menu, allowing configuring the different light source.
  Options
};

// Models a position and rotation of an object in the scene.
struct Transform {
  // position of the object in world cooridinates.
  Point position;

  // The rotation of the object.
  Quaternion quaternion;

  Transform(Point position, Quaternion quaternion)
      : position(position), quaternion(quaternion) {}
};

struct Robot {
  // transform of the robot.
  Transform transform;

  // This rotation is relative to the torso.
  Quaternion headRotationRelativeToTransform;

  // Relative to the left shoulder pivot point.
  Quaternion armRotation;

  // Relative to the elbow pivot point.
  Quaternion elbowRotation;

  // Relative to the hand pivot point.
  Quaternion handRotation;

  Robot(Transform transform, Quaternion headRotation, Quaternion armRotation,
        Quaternion elbowRotation, Quaternion handRotation)
      : transform(transform), headRotationRelativeToTransform(headRotation),
        armRotation(armRotation), elbowRotation(elbowRotation),
        handRotation(handRotation) {}
};

/// The possible keyboard commands that are currently active.
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

struct ViewportInfo {
  GLint x;
  GLint y;
  GLsizei width;
  GLsizei hight;
};

struct State {
  // the current point of view of the camera.
  PointOfView pointOfView;

  // The current mode of control, for example, if the control mode is Robot, the
  // arrow keys will move the robot.
  ControlMode controlMode;

  // The current keyboard commands that are active.
  ControlCommands controlCommands;

  // The current state of the robot in the
  struct Robot robot;

  // The current state of the camera.
  Transform camera;

  int windowHeight;
  int windowWidth;

  ViewportInfo viewportInfo;

  // true if and only if the menu is currently being shown.
  bool showingMenu;

  // true if and only if the debug info should be shown.
  bool displayDebugInfo;

  // The current active menu.
  ActiveMenue activeMenue;

  // Current color and intensity of the global ambient light source.
  GLint AmbientR;
  GLint AmbientG;
  GLint AmbientB;
  GLint AmbientI;

  // current color, intensity and position of the lamp (the point light source).
  GLint PointLightR;
  GLint PointLightG;
  GLint PointLightB;
  GLint PointLightI;
  Transform lampPosition;

  State()
      : pointOfView(ThirdPerson), controlMode(Robot), controlCommands(),
        robot({{0, 0, 0}, {1, 0, 0}}, Quaternion::identity(),
              Quaternion::identity(), Quaternion::identity(),
              Quaternion::identity()),
        camera({{5, 5, 5}, Quaternion(0.88, -0.325, 0.325, 0)}),
        displayDebugInfo(false), activeMenue(None), AmbientR(255),
        AmbientG(255), AmbientB(255), AmbientI(150), PointLightR(255),
        PointLightG(255), PointLightB(255), PointLightI(255),
        lampPosition({-5, 3, 0}, {1, 0, 0}) {}
};

void updatedState(State &currentState, double deltaTime);
void moveForwardAndBackwords(Transform &t, ControlCommands controlCommands,
                             bool shouldInvertForwardDirection,
                             double deltaTime);
void moveLeftAndRight(Transform &t, const ControlCommands &controlCommands,
                      bool shouldInvertDirection, double deltaTime);
void moveUpAndDown(Transform &t, const ControlCommands &controlCommands,
                   double deltaTime);
void rotateLeftAndRight(Quaternion &quaternion,
                        const ControlCommands &controlCommands,
                        double deltaTime, double rotationSpeed);
void rotateUpAndDown(Quaternion &quaternion,
                     const ControlCommands &controlCommands, double deltaTime,
                     double rotationSpeed);

// Updates the state of the scene according to the current control commands.
// The control mode controls which part of the scene is currently being
// manipulated, some objects can be manipulated in different ways, for example -
// the robot head can only rotate, while the robot itself can move and also
// rotate.
inline void updatedState(State &currentState, double deltaTime) {
  switch (currentState.controlMode) {
  case Robot:
    moveForwardAndBackwords(currentState.robot.transform,
                            currentState.controlCommands, false, deltaTime);
    moveLeftAndRight(currentState.robot.transform, currentState.controlCommands,
                     false, deltaTime);
    rotateLeftAndRight(currentState.robot.transform.quaternion,
                       currentState.controlCommands, deltaTime,
                       ROBOT_ROTATION_SPEED);
    break;
  case RobotHead:
    rotateLeftAndRight(currentState.robot.headRotationRelativeToTransform,
                       currentState.controlCommands, deltaTime,
                       ROBOT_ROTATION_SPEED);
    rotateUpAndDown(currentState.robot.headRotationRelativeToTransform,
                    currentState.controlCommands, deltaTime,
                    CAMERA_ROTATION_SPEED);
    break;
  case Camera:
    moveForwardAndBackwords(currentState.camera, currentState.controlCommands,
                            true, deltaTime);
    moveLeftAndRight(currentState.camera, currentState.controlCommands, true,
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
  case Lamp:
    moveForwardAndBackwords(currentState.lampPosition,
                            currentState.controlCommands, false, deltaTime);
    moveLeftAndRight(currentState.lampPosition, currentState.controlCommands,
                     false, deltaTime);
    moveUpAndDown(currentState.lampPosition, currentState.controlCommands,
                  deltaTime);
    break;
  }
  currentState.windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
  currentState.windowWidth = glutGet(GLUT_WINDOW_WIDTH);
}

// The following functions are used to move and rotate the different objects in
// the scene, using the delta time and speed to calculate by how much to update.

inline void moveForwardAndBackwords(Transform &t,
                                    ControlCommands controlCommands,
                                    bool shouldInvertForwardDirection,
                                    double deltaTime) {
  // We expose the shouldInvertForwardDirection because we want to invert the
  // controls when controlling the camera, because the camera looks at the
  // neagtive z axis, making controls feel "backwords" if we don't invert them.
  Point forward =
      t.quaternion.getForwardVector() * (shouldInvertForwardDirection ? -1 : 1);

  if (controlCommands.isMovingForward) {
    t.position += forward * MOVE_SPEED * deltaTime;
  } else if (controlCommands.isMovingBackward) {
    t.position -= forward * MOVE_SPEED * deltaTime;
  }
}

inline void moveLeftAndRight(Transform &t,
                             const ControlCommands &controlCommands,
                             bool shouldInvertDirection, double deltaTime) {
  Point right =
      t.quaternion.getRightVector() * (shouldInvertDirection ? -1 : 1);
  if (controlCommands.isMovingLeft) {
    t.position += right * MOVE_SPEED * deltaTime;
  } else if (controlCommands.isMovingRight) {
    t.position -= right * MOVE_SPEED * deltaTime;
  }
}

inline void moveUpAndDown(Transform &t, const ControlCommands &controlCommands,
                          double deltaTime) {
  if (controlCommands.isMovingUp) {
    t.position += t.quaternion.getUpVector() * MOVE_SPEED * deltaTime;
  } else if (controlCommands.isMovingDown) {
    t.position -= t.quaternion.getUpVector() * MOVE_SPEED * deltaTime;
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
#endif
