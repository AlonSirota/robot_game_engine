#include "Quaternion.h"
#include "State.h"
#include "ui/ui.hpp"
#include <GL/freeglut_std.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define TARGET_FRAME_RATE 60
#define TARGET_DELTA_BETWEEN_FRAMES_MS (1000.0 / TARGET_FRAME_RATE);
#define CHECKERBOARD_SIZE 4

GLuint floorTextureId;
void renderFloor();
void setupViewport();
void clearMatrices();
void pushMatrices();
void popMatrices();
void setupCamera(Transform camera);
void displayFunc();
void displayRobot(struct Robot robot);
void drawAxis();
void keyboardFunc(unsigned char key, int x, int y);
void keyboardUpFunc(unsigned char key, int x, int y);
void idleFunc();
void PostRedisplayWrapper(int);
void displayUI();

double presentedTime = 0;
State state = State();

UIManager uiManager(&state);

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
  gluPerspective(45, RENDER_ASPECT_RATIO, 1, 100);
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

  setupViewport();
  glClearColor(0.5, 0.8, 0.8, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  clearMatrices();
  setupCamera(state.camera);
  setupProjection();
  drawAxis();
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
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  if (state.windowHeight * RENDER_ASPECT_RATIO < state.windowWidth) {
    int w = state.windowHeight *
            (RENDER_ASPECT_RATIO); // w is width adjusted for aspect ratio
    int left = (state.windowWidth - w) / 2;
    glViewport(
        left, 0, w,
        state.windowHeight); // fix up the viewport to maintain aspect ratio
  } else {
    int h = state.windowWidth *
            (1.0 / RENDER_ASPECT_RATIO); // w is width adjusted for aspect ratio
    int bottom = (state.windowHeight - h) / 2;
    glViewport(0, bottom, state.windowWidth,
               h); // fix up the viewport to maintain aspect ratio
  }
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

void glVertexPoint(Point p) { glVertex3f(p.x, p.y, p.z); }

void glTranslatePoint(Point p) { glTranslatef(p.x, p.y, p.z); }

void displayRobotTorso() {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glColor3f(0.5, 0.5, 0.5);
  glScalef(1, 2, 0.5);
  glutSolidCube(1);
  glPopMatrix();
}

void displayRobotHead(Quaternion quaternion) {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  // Move the head above the torso
  glTranslatef(0, 1.5, 0);
  glMultMatrixf(quaternion.toMatrix());

  // Draw the head (grey sphere)
  glColor3f(0.7, 0.7, 0.7); // Grey color
  glutSolidSphere(
      0.5, 10,
      10); // Radius 0.5, 10 slices and stacks for a smooth enough shape.

  // Now I'll draw the eyes.
  glColor3f(1.0, 0, 0); // Red color

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
void displayRobotArm(Quaternion armRotation, Quaternion elbowRotation, Quaternion handRotation) {
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
  glColor3f(0.8, 0.3, 0.3);
  glutSolidCube(1);
  glPopMatrix();

  // Move to elbow position
  glTranslatef(0, -upperArmLength, 0);

  // Elbow rotation
  // Because `glutSolidCube` draws the cube centered at the origin, in order to
  // align the cube pivot point with the elbow we translate by half of the
  // length.
  glTranslatef(0, upperArmLength / 2, 0); // Align cube center to elbow pivot point.
  glMultMatrixf(elbowRotation.toMatrix()); // Rotate around the elbow.
  glTranslatef(0, -upperArmLength / 2, 0); // Translate back.

  // Draw lower arm
  glPushMatrix();
  glColor3f(0.2, 0.8, 0.2);
  glScalef(0.15, lowerArmLength, 0.15);
  glutSolidCube(1);
  glPopMatrix();

  // Move to wrist position
  glTranslatef(0, -lowerArmLength, 0);

  // Hand rotation
  glTranslatef(0, handSize / 2, 0); // Align cube center to wrist pivot point
  glMultMatrixf(handRotation.toMatrix()); // Rotate around the wrist
  glTranslatef(0, -handSize / 2, 0); // Translate back

  // Draw hand
  glPushMatrix();
  glColor3f(0.9, 0.7, 0.5); // Skin color for the hand
  glutSolidCube(handSize);
  glPopMatrix();

  glPopMatrix();
}

void displayRobot(struct Robot robot) {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatePoint(robot.transform.position);
  glMultMatrixf(robot.transform.quaternion.toMatrix());

  displayRobotTorso();
  displayRobotHead(robot.headRotation);
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

  glColor3f(1, 1, 1);
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

  initFloorTexture();
  glutDisplayFunc(displayFunc);
  glutIdleFunc(idleFunc);
  glutKeyboardFunc(keyboardFunc);
  glutKeyboardUpFunc(keyboardUpFunc);
  glutSpecialFunc(specialKeyboardKeysFunc);
  glutSpecialUpFunc(specialKeyboardKeysUpFunc);

  glutMainLoop();
  return 0;
}
