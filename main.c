#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glut.h>

void redraw(void) {
  glPointSize(3.2);
  glClearColor(0.0, 0.3, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_POINTS);

    glColor3f(1.0, 0.0, 1.0);
    glVertex2f(0.0, 0.0);

  glEnd();
  glFlush();
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB);
  glutInitWindowPosition(300, 100);
  glutInitWindowSize(150, 150);
  int window = glutCreateWindow(argv[0]);
  glutDisplayFunc(redraw);
  glutMainLoop();
  return 0;
}
