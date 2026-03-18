#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glut.h>

void redraw(void) {
  glPointSize(3.2);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_QUADS);

    glColor3f(1.0, 1.0, 1.0);
      glVertex2f(0.8, 0.2);
    glColor3f(1.0, 0.0, 0.0);
      glVertex2f(0.7, -0.5);
    glColor3f(0.0, 1.0, 0.0);
      glVertex2f(-0.1, -0.5);
      glVertex2f(0.5, 0.2);
    
    glColor3f(1.0, 1.0, 0.0);
      glVertex2f(0.6, 0.8);
      glVertex2f(-0.5, -0.6);
    glColor3f(0.0, 0.0, 1.0);
      glVertex2f(-0.7, 0.0);
      glVertex2f(-0.5, 0.6);

  glEnd();
  glFlush();
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB);
  glutInitWindowPosition(300, 100);
  glutInitWindowSize(520, 520);
  int window = glutCreateWindow(argv[0]);
  glutDisplayFunc(redraw);
  glutMainLoop();
  return 0;
}
