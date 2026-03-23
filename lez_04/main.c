#include "checkerror.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>


void redraw(void) {
  glPointSize(3.2);
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
  checkError("redraw");
  glFlush();
}


void keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case ' ': // SPACE key
      glClearColor((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 1.0);
      glutPostRedisplay();
      break;
    case 27: // ESC key
      exit(0);
  }
}

int main(int argc, char **argv) {
  int window;
  srand(time(NULL));
  
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB);
  glutInitWindowPosition(300, 100);
  glutInitWindowSize(520, 520);

  window = glutCreateWindow(argv[0]);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glutDisplayFunc(redraw);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  
  return 0;
}
