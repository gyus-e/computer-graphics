#include <GL/gl.h>
#include <GL/glut.h>

void redraw(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_POINTS);
  glVertex2f(0.0, 0.0);
  glEnd();
  glFlush();
}

int main (int argc, char **argv){
  glutInit(&argc, argv);
  glutCreateWindow("Window Title");
  glutDisplayFunc(redraw);
  glutMainLoop();
  return 0;
}