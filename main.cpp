#include <GL/freeglut_std.h>
#include <GL/glut.h>

void displayFunc();
void displayHead();
void drawAxis();

void drawAxis() {
  glBegin(GL_LINES);
  // x-axis (red)
  glColor3f(1, 0, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(5, 0, 0);

  // y-axis (green)
  glColor3f(0, 1, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 5, 0);

  // z-axis (blue)
  glColor3f(0, 0, 1);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, 5);
  glEnd();
}

void setupOrthographicProjection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-2, 2, -2, 2, -2, 2);
}

void displayFunc() {
  glClear(GL_COLOR_BUFFER_BIT);
  setupOrthographicProjection();
  drawAxis();
  displayHead();
  glFlush();
}

void displayHead() {
  glutSolidTeapot(1);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitWindowSize(1000, 1000);
  glutCreateWindow("Final project.");

  glutDisplayFunc(displayFunc);

  glutMainLoop();
  return 0;
}
