#include "Quaternion.h"
#include "State.h"
#include "ui/ui.hpp"
#include <GL/freeglut_std.h>
#include <GL/glu.h>
#include <GL/glut.h>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE GLUT_MULTISAMPLE
#endif

#define TARGET_FRAME_RATE 60
#define TARGET_DELTA_BETWEEN_FRAMES_MS (1000.0 / TARGET_FRAME_RATE);
#define CHECKERBOARD_SIZE 4
#define ROBOT_HEAD_HEIGHT 1.0

GLuint floorTextureId;

GLfloat redColor[] = {1.0f, 0.0f, 0.0f, 1.0f};
GLfloat greenColor[] = {0.0f, 1.0f, 0.0f, 1.0f};
GLfloat blueColor[] = {0.0f, 0.0f, 1.0f, 1.0f};
GLfloat whiteColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat gray1Color[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat gray2Color[] = {0.7f, 0.7f, 0.7f, 1.0f};

GLfloat clawColor[] = {0.2f, 0.2f, 1.0f, 1.0f};
GLfloat upperArmColor[] = {0.8f, 0.3f, 0.3f, 1.0f};
GLfloat lowerArmColor[] = {0.2f, 0.8f, 0.2f, 1.0f};

void renderFloor();
void setupViewport();
void clearMatrices();
void pushMatrices();
void popMatrices();
void setupLighting();
void setupCamera(Transform camera, struct Robot robot, PointOfView pov);
void displayFunc();
void displayRobot(struct Robot robot);
void keyboardFunc(unsigned char key, int x, int y);
void keyboardUpFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void idleFunc();
void PostRedisplayWrapper(int);
void displayUI();

double presentedTime = 0;
State state = State();

UIManager uiManager(&state);

void setupProjection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, RENDER_ASPECT_RATIO, 1, 100);
}

void gluLookAt(Point camera, Point target, Point up) {
  gluLookAt(camera.x, camera.y, camera.z, target.x, target.y, target.z, up.x,
            up.y, up.z);
}

void setupCamera(Transform camera, struct Robot robot, PointOfView pov) {
  glMatrixMode(GL_MODELVIEW);

  // (0,0,0 are placeholder values, they are initialized in the switch below.
  Point up = Point(0, 0, 0);
  Point pointOfViewPosition = Point(0, 0, 0);
  Point direction = Point(0, 0, 0);
  Point target = Point(0, 0, 0);

  switch (pov) {
  case FirstPerson:
    // We need to put the point of view position above the robot, otherwise the
    // head will cover the screen.
    pointOfViewPosition =
        robot.transform.position + Point(0, ROBOT_HEAD_HEIGHT, 0);
    // We multiply the robot.transform.quaternion by the head rotation to get
    // the absolute orientation of the head, which is the direction of the
    // camera.
    direction =
        (robot.headRotationRelativeToTransform * robot.transform.quaternion)
            .getForwardVector();
    up = Point(0, 1, 0);
    break;
  case ThirdPerson:
    pointOfViewPosition = camera.position;
    // We multiply the direction by -1 because the camera looks in the
    // direaction of the camera's negative z-axis.
    direction = camera.quaternion.getForwardVector() * -1;
    up = camera.quaternion.getUpVector();
    break;
  }

  target = pointOfViewPosition + direction;
  gluLookAt(pointOfViewPosition, target, up);
}

void displayFunc() {
  double frameStartTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;

  glClearColor(0.5, 0.8, 0.8, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  setupViewport();
  clearMatrices();
  setupLighting();
  setupCamera(state.camera, state.robot, state.pointOfView);
  setupProjection();
  displayRobot(state.robot);
  renderFloor();
  displayUI();

  glFlush();

  double frameEndTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
  double delta = frameEndTime - frameStartTime;
  double timeToNextFrame = TARGET_DELTA_BETWEEN_FRAMES_MS - delta;

  if (timeToNextFrame <= 0) {
    timeToNextFrame = 0; // TODO: mark frame skipped, add to log
  }

  glutTimerFunc(timeToNextFrame, PostRedisplayWrapper, 0);
}

void setupViewport() {
  int width = state.windowWidth,
    left = 0,
    hight = state.windowHeight,
    bottom = 0;

  if (state.windowHeight * RENDER_ASPECT_RATIO < state.windowWidth) {
    width = state.windowHeight *
            (RENDER_ASPECT_RATIO); // w is width adjusted for aspect ratio
    left = (state.windowWidth - width) / 2;
  } else {
    hight = state.windowWidth *
            (1.0 / RENDER_ASPECT_RATIO); // w is width adjusted for aspect ratio
    bottom = (state.windowHeight - hight) / 2;
  }

  glViewport(left, bottom, width, hight); // fix up the viewport to maintain aspect ratio
  state.viewportInfo.x = left;
  state.viewportInfo.y = bottom;
  state.viewportInfo.width = width;
  state.viewportInfo.hight = hight;
}

void clearMatrices() {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
}

void pushMatrices() {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
}

void popMatrices() {
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
}

void setupAmbientLighting(){
  GLfloat intensityMultipler = state.AmbientI / 255.0f;
  GLfloat ambiant[] = {state.AmbientR / 255.0f * intensityMultipler, state.AmbientG / 255.0f * intensityMultipler, state.AmbientB / 255.0f * intensityMultipler, 1.0f};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambiant);
}

void setupLighting() {
  glEnable(GL_LIGHTING);
  setupAmbientLighting();
}
 
void glVertexPoint(Point p) { glVertex3f(p.x, p.y, p.z); }

void glTranslatePoint(Point p) { glTranslatef(p.x, p.y, p.z); }

void displayRobotTorso() {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, gray1Color);
  glScalef(1, 2, 0.5);
  glutSolidCube(1);
  glPopMatrix();
}

void displayRobotHead(Quaternion quaternion) {
  GLfloat headRadius = ROBOT_HEAD_HEIGHT / 2;
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  // Move the head above the torso
  glTranslatef(0, 1.5, 0);
  glMultMatrixf(quaternion.toMatrix());

  // Draw the head (grey sphere)
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, gray2Color);
  glutSolidSphere(
      headRadius, 10,
      10); // Radius 0.5, 10 slices and stacks for a smooth enough shape.

  // Now I'll draw the eyes.
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, redColor);

  // Right eye
  glPushMatrix();
  glTranslatef(0.2, 0.1, 0.4);
  glRotatef(90, 1, 0, 0);                  // Rotate to face front
  gluDisk(gluNewQuadric(), 0, 0.1, 20, 1); // Inner radius 0, outer radius 0.1
  glPopMatrix();

  // Left eye
  glPushMatrix();
  glTranslatef(-0.2, 0.1, 0.4);
  glRotatef(90, 1, 0, 0);
  gluDisk(gluNewQuadric(), 0, 0.1, 20, 1);
  glPopMatrix();
  glPopMatrix();
}

void displayClaw() {
  GLfloat clawLength = 0.4;
  GLfloat clawWidth = 0.08;
  GLfloat clawDepth = 0.1;
  GLfloat clawOpennessDegrees = 15;

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, clawColor);

  // Left prong.
  glPushMatrix();
  glRotatef(-clawOpennessDegrees, 0, 0,
            1); // Rotate away from the wrist's axis.
  glTranslatef(0, -clawLength / 2,
               0); // Translate away from the wrist, because SolidCubes with the
                   // current pivot point as the center.
  glScalef(clawWidth, clawLength, clawDepth);
  glutSolidCube(1.0);
  glPopMatrix();

  // Right prong.
  glPushMatrix();
  glRotatef(clawOpennessDegrees, 0, 0, 1);
  glTranslatef(0, -clawLength / 2,
               0); // Translate away from the wrist, because SolidCubes with the
                   // current pivot point as the center.
  glScalef(clawWidth, clawLength, clawDepth);
  glutSolidCube(1.0);
  glPopMatrix();
}

void displayRobotArm(Quaternion armRotation, Quaternion elbowRotation,
                     Quaternion handRotation) {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  GLfloat upperArmLength = 0.4;
  GLfloat lowerArmLength = 0.3;
  GLfloat handSize = 0.25;
  GLfloat fingerLength = 0.1;

  // Upper arm, from shoulder to elbow
  glTranslatef(0.65, 0.8, 0); // Position the arm on the side of the torso

  // Shoulder rotation
  glMultMatrixf(armRotation.toMatrix());

  // Draw upper arm
  glPushMatrix();
  glScalef(0.2, upperArmLength, 0.2);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, upperArmColor);
  glutSolidCube(1);
  glPopMatrix();

  // Move to elbow position
  glTranslatef(0, -upperArmLength, 0);

  // Elbow rotation
  // Because `glutSolidCube` draws the cube centered at the origin, in order to
  // align the cube pivot point with the elbow we translate by half of the
  // length.
  glTranslatef(0, upperArmLength / 2,
               0); // Align cube center to elbow pivot point.
  glMultMatrixf(elbowRotation.toMatrix()); // Rotate around the elbow.
  glTranslatef(0, -upperArmLength / 2, 0); // Translate back.

  // Draw lower arm
  glPushMatrix();
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, lowerArmColor);
  glScalef(0.15, lowerArmLength, 0.15);
  glutSolidCube(1);
  glPopMatrix();

  // Move to wrist position
  glTranslatef(0, -lowerArmLength / 2, 0);
  glMultMatrixf(handRotation.toMatrix());
  displayClaw();

  glPopMatrix();
}

void displayRobot(struct Robot robot) {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatePoint(robot.transform.position);
  glMultMatrixf(robot.transform.quaternion.toMatrix());

  displayRobotTorso();
  displayRobotHead(robot.headRotationRelativeToTransform);
  displayRobotArm(robot.armRotation, robot.elbowRotation, robot.handRotation);

  glPopMatrix();
}
void displayUI() {
  pushMatrices();
  clearMatrices();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, RENDER_WIDTH, 0, RENDER_HIGHT);

  uiManager.Draw();

  popMatrices();
};

void renderFloor() {
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, floorTextureId);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glScalef(10, 1, 10);

  // The texture is configured to repeat.
  int textureRepeatCount = 4;
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, whiteColor);
  glBegin(GL_QUADS);
  glNormal3f(0, 1, 0);
  glTexCoord2f(0, 0);
  glVertex3f(-1, 0, -1);
  glTexCoord2f(textureRepeatCount, 0);
  glVertex3f(1, 0, -1);
  glTexCoord2f(textureRepeatCount, textureRepeatCount);
  glVertex3f(1, 0, 1);
  glTexCoord2f(0, textureRepeatCount);
  glVertex3f(-1, 0, 1);
  glEnd();

  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}

void keyboardFunc(unsigned char key, int x, int y) {
  if(key == 27){
    if(state.activeMenue == None){
      state.activeMenue = Main;
    }else{
      state.activeMenue = None;
    }
  }
  uiManager.KeyPress(key);
  if(state.activeMenue != None){return;}

  switch (key) {
  case '1':
    state.controlMode = Camera;
    break;
  case '2':
    state.controlMode = Robot;
    break;
  case '3':
    state.controlMode = RobotHead;
    break;
  case '4':
    state.controlMode = UpperArm;
    break;
  case '5':
    state.controlMode = LowerArm;
    break;
  case '6':
    state.controlMode = Hand;
    break;
  case ' ':
    // toggle between first person and third person.
    state.pointOfView =
        state.pointOfView == FirstPerson ? ThirdPerson : FirstPerson;
    break;
  case 'w':
    state.controlCommands.isMovingForward = true;
    break;
  case 'a':
    state.controlCommands.isMovingLeft = true;
    break;
  case 's':
    state.controlCommands.isMovingBackward = true;
    break;
  case 'd':
    state.controlCommands.isMovingRight = true;
    break;
  case 'q':
    state.controlCommands.isMovingUp = true;
    break;
  case 'e':
    state.controlCommands.isMovingDown = true;
    break;
  }
}

void keyboardUpFunc(unsigned char key, int x, int y) {
  switch (key) {
  case 'w':
    state.controlCommands.isMovingForward = false;
    break;
  case 'a':
    state.controlCommands.isMovingLeft = false;
    break;
  case 's':
    state.controlCommands.isMovingBackward = false;
    break;
  case 'd':
    state.controlCommands.isMovingRight = false;
    break;
  case 'q':
    state.controlCommands.isMovingUp = false;
    break;
  case 'e':
    state.controlCommands.isMovingDown = false;
    break;
  }
}

void specialKeyboardKeysFunc(int key, int x, int y) {
  switch (key) {
  case GLUT_KEY_UP:
    state.controlCommands.isRotatingUp = true;
    break;
  case GLUT_KEY_DOWN:
    state.controlCommands.isRotatingDown = true;
    break;
  case GLUT_KEY_LEFT:
    state.controlCommands.isRotatingLeft = true;
    break;
  case GLUT_KEY_RIGHT:
    state.controlCommands.isRotatingRight = true;
    break;
  }
}

void specialKeyboardKeysUpFunc(int key, int x, int y) {
  switch (key) {
  case GLUT_KEY_UP:
    state.controlCommands.isRotatingUp = false;
    break;
  case GLUT_KEY_DOWN:
    state.controlCommands.isRotatingDown = false;
    break;
  case GLUT_KEY_LEFT:
    state.controlCommands.isRotatingLeft = false;
    break;
  case GLUT_KEY_RIGHT:
    state.controlCommands.isRotatingRight = false;
    break;
  }
}

void adjustMouseCoordToViewPort(int *mX, int *mY){
  *mY = state.windowHeight - *mY;

  *mX -= state.viewportInfo.x;
  *mY -= state.viewportInfo.y;

  *mX *= 1920.0 / state.viewportInfo.width;
  *mY *= 1080.0 / state.viewportInfo.hight;
  
}

void mouseFunc(int button, int mState, int mX, int mY){
  adjustMouseCoordToViewPort(&mX, &mY);

  //is in viewport
  if(mX >= 0 &&
      mX <= 1920 &&
      mY >= 0 &&
      mY <= 1080){
          
          if(button == GLUT_LEFT_BUTTON && mState == GLUT_DOWN){
            uiManager.MouseClick(mX, mY);
          }
  }
  
}

void idleFunc() {
  double currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
  double deltaTime = currentTime - presentedTime;

  updatedState(state, deltaTime);

  presentedTime = currentTime;
}

void PostRedisplayWrapper(int) { glutPostRedisplay(); }

void initFloorTexture() {
  // Initialize checkerboard matrix with white and black squares.
  GLubyte checkerboard[CHECKERBOARD_SIZE][CHECKERBOARD_SIZE][3];
  for (int i = 0; i < CHECKERBOARD_SIZE; i++) {
    for (int j = 0; j < CHECKERBOARD_SIZE; j++) {
      GLubyte intensity = ((i + j) % 2 == 0) ? 255 : 0;
      checkerboard[i][j][0] = intensity; // Red
      checkerboard[i][j][1] = intensity; // Green
      checkerboard[i][j][2] = intensity; // Blue
    }
  }

  glGenTextures(1, &floorTextureId);
  glBindTexture(GL_TEXTURE_2D, floorTextureId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, CHECKERBOARD_SIZE, CHECKERBOARD_SIZE,
               0, GL_RGB, GL_UNSIGNED_BYTE, checkerboard);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitWindowSize(600, 1000);
  glutCreateWindow("Final project.");
  glutInitDisplayMode(GLUT_DEPTH);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_BLEND); //Enable blending.
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function.

  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  


  initFloorTexture();
  glutDisplayFunc(displayFunc);
  glutIdleFunc(idleFunc);
  glutKeyboardFunc(keyboardFunc);
  glutKeyboardUpFunc(keyboardUpFunc);
  glutMouseFunc(mouseFunc);
  glutSpecialFunc(specialKeyboardKeysFunc);
  glutSpecialUpFunc(specialKeyboardKeysUpFunc);

  glutMainLoop();
  return 0;
}
