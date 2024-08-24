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
GLfloat metal_ambient[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat metal_diffuse[] = {0.7f, 0.7f, 0.7f, 1.0f};
GLfloat metal_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat metal_shininess = 20.0f;

GLfloat clawColor[] = {0.2f, 0.2f, 1.0f, 1.0f};
GLfloat upperArmColor[] = {0.8f, 0.3f, 0.3f, 1.0f};
GLfloat lowerArmColor[] = {0.2f, 0.8f, 0.2f, 1.0f};

void drawWoodenDoor(); // Add this line to draw the door
void renderFloor();
void drawLamp();
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

void drawTeapot() {
    glPushMatrix();

    glTranslatef(2, 0, 2);
    
    // Set up metallic material properties
    glColor4fv(metal_ambient);
    GLfloat metal_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat metal_diffuse[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat metal_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat metal_shininess = 200.0f;  // Increased for a more polished look

    glColor3f(0.85, 0.85, 0.85);

    // Apply material properties
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, metal_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, metal_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, metal_specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, metal_shininess);

    // Position and draw the teapot
    glTranslatef(0, 1, 0);
    glutSolidTeapot(1);

    glPopMatrix();
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
  renderFloor();
  drawTeapot();
  drawLamp();
  drawWoodenDoor();
  displayRobot(state.robot);
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
  int width = state.windowWidth, left = 0, hight = state.windowHeight,
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

  glViewport(left, bottom, width,
             hight); // fix up the viewport to maintain aspect ratio
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

void setupAmbientLighting() {
  GLfloat intensityMultipler = state.AmbientI / 255.0f;
  GLfloat ambiant[] = {state.AmbientR / 255.0f * intensityMultipler,
                       state.AmbientG / 255.0f * intensityMultipler,
                       state.AmbientB / 255.0f * intensityMultipler, 1.0f};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambiant);
}

void setupLighting() {
  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  setupAmbientLighting();

  /// The first three numbers are the xyz, and the 4th number is a boolean
  /// controlling if this point light is infinitely far away or not.
  GLfloat purpleLampColor[] = {1, 0, 1, 1};
  glEnable(GL_LIGHT0);
  GLfloat lightPosition[] = {0, 0, 0, 1};
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, purpleLampColor);
  glLightfv(GL_LIGHT0, GL_SPECULAR, purpleLampColor);

  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 2);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.002);
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01);
}

void glVertexPoint(Point p) { glVertex3f(p.x, p.y, p.z); }

void glTranslatePoint(Point p) { glTranslatef(p.x, p.y, p.z); }

void drawLamp() {
  glPushMatrix();
  // Position the lamp on the floor, in some arbitrary xz po
  glTranslatef(-3, 0, -4);

  // Draw the base (cylinder)
  GLUquadricObj *quadric = gluNewQuadric();
  glColor3f(0.4f, 0.4f, 0.4f); // Dark gray color for the base
  glPushMatrix();
  glRotatef(-90, 1, 0, 0); // Rotate to stand upright
  gluCylinder(quadric, 0.2, 0.2, 0.5, 20,
              1); // radius_top, radius_bottom, height, slices, stacks
  glPopMatrix();

  // Draw the lampshade (cone)
  glColor3f(0.9f, 0.9f, 0.8f); // Light beige color for the lampshade
  glPushMatrix();
  glTranslatef(0, 0.5, 0); // Move up to the top of the base
  glRotatef(-90, 1, 0, 0); // Rotate to point upwards
  gluCylinder(quadric, 0.4, 0.1, 0.4, 20,
              1); // radius_bottom, radius_top, height, slices, stacks
  glPopMatrix();

  gluDeleteQuadric(quadric);

  setupLighting();
  glPopMatrix();
}

void drawWoodenDoor() {
  glPushMatrix();

  // Position the door
  glTranslatef(2, 1, -7);

  // Set material properties for wood
  // these colors are low intensity to model wood in phong's illumination mode.
  // (no specular lighting).
  GLfloat wood_ambient_and_diffuse[] = {0.6f, 0.3f, 0.1f, 1.0f};
  GLfloat wood_specular[] = {0.0f, 0.0f, 0.0f, 0.0f};
  GLfloat wood_shininess = 0.0f;

  glColor4fv(wood_ambient_and_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, wood_ambient_and_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, wood_ambient_and_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, wood_specular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, wood_shininess);

  // Door frame
  glPushMatrix();
  glScalef(1.1f, 2.2f, 0.1f);
  glutSolidCube(1.0);
  glPopMatrix();

  // Door door knob (metallic sphere)
  glColor4fv(metal_ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, metal_ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, metal_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, metal_specular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, metal_shininess);

  glPushMatrix();
  glTranslatef(0.35, 0, 0.1);
  glutSolidSphere(0.09, 20, 20);
  glPopMatrix();

  glPopMatrix();
}

void displayRobotTorso() {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glColor3fv(gray1Color);
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
  glColor3fv(gray2Color);
  glutSolidSphere(
      headRadius, 10,
      10); // Radius 0.5, 10 slices and stacks for a smooth enough shape.

  // Now I'll draw the eyes.
  glColor3fv(redColor);

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

  glColor3fv(clawColor);

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
  glColor3fv(upperArmColor);
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
  glColor3fv(lowerArmColor);
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

  // configure the floor to be shiny
  GLfloat floorSpecularColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, floorSpecularColor);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 5.0f);
  glColor3fv(whiteColor);

  glScalef(10, 1, 10);

  // The texture is configured to repeat.
  int textureRepeatCount = 4;
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

  // turn off the specular that was used for the floor, so we won't affect other
  // objects in the scene.
  GLfloat zeroIntensityColor[] = {0.0f, 0.0f, 0.0f, 0.0f};
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zeroIntensityColor);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);

  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}

void keyboardFunc(unsigned char key, int x, int y) {
  if (key == 27) {
    if (state.activeMenue == None) {
      state.activeMenue = Main;
    } else {
      state.activeMenue = None;
    }
  }
  uiManager.KeyPress(key);
  if (state.activeMenue != None) {
    return;
  }

  switch (key) {
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

void adjustMouseCoordToViewPort(int *mX, int *mY) {
  *mY = state.windowHeight - *mY;

  *mX -= state.viewportInfo.x;
  *mY -= state.viewportInfo.y;

  *mX *= 1920.0 / state.viewportInfo.width;
  *mY *= 1080.0 / state.viewportInfo.hight;
}

void mouseFunc(int button, int mState, int mX, int mY) {
  adjustMouseCoordToViewPort(&mX, &mY);

  // is in viewport
  if (mX >= 0 && mX <= 1920 && mY >= 0 && mY <= 1080) {

    if (button == GLUT_LEFT_BUTTON && mState == GLUT_DOWN) {
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
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_BLEND);                                // Enable blending.
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set blending function.

  // This makes specular lighting respect the current point of view.
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
