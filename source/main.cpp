#include <GL/freeglut_std.h>
#include <GL/glu.h>
#include <GL/glut.h>

void setupCamera();
void displayFunc();
void displayHead();
void drawAxis();

void drawAxis() {
  glBegin(GL_LINES);
  // y-axis (green)
  glColor3f(0, 1, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 5, 0);

  // x-axis (red)
  glColor3f(1, 0, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(5, 0, 0);

  // z-axis (blue)
  glColor3f(0, 0, 1);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, 5);
  glEnd();
}

void setupProjection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, 1, 1, 100);
}

void setupCamera() {
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(0, 0, 10, 0, 0, 1, 0, 1, 0);
}

void displayFunc() {
  glClear(GL_COLOR_BUFFER_BIT);
  setupCamera();
  setupProjection();
  drawAxis();
  displayHead();
  glFlush();
}

void displayHead() {
  glMatrixMode(GL_MODELVIEW);
  glutSolidTetrahedron();
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitWindowSize(1000, 1000);
  glutCreateWindow("Final project.");

  glutDisplayFunc(displayFunc);

  glutMainLoop();
  return 0;
}
