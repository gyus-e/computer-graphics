#include <math.h>
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glut.h>

/**
Creare un prisma regolare con base ottagonale avente
il centro della base inferiore in (0.2, 0.2, -0.5),
il centro della base superiore in (0.2, 0.2, -1.5)
e basi circoscritte dalla circonferenza di raggio 0.5

Utilizzando in maniera combinata le callback per mouse e tastiera,
rendere modificabili i tre valori di altezza larghezza e profondità
del Volume di Visa prendendo come valori iniziali quelli di:
glOrtho(-1, 1, -1, 1, 1, 5)
*/
GLdouble view[6] = {-1.0, 1.0, -1.0, 1.0, 1.0, 5.0};
GLdouble translate[3] = {0.0, 0.0, -3.0};

GLvoid drawOctagon(const GLdouble centro[3], const GLdouble raggio, const GLdouble rgb[3]) {
  const GLint sides = 8;
  GLdouble angle, x, y, z;

  glColor3f(rgb[0], rgb[1], rgb[2]);
  glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.2, 0.2, -1.5);
    for (int i = 0; i <= sides; i++) {
      angle = 2 * M_PI * i / sides;
      x  = centro[0] + raggio * cos(angle);
      y  = centro[1] + raggio * sin(angle);
      z  = centro[2];
      glVertex3f(x, y, z);
    }
  glEnd();
}

GLvoid display( GLvoid ) {
  const GLdouble centro_base_inf[] = {0.2, 0.2, -0.5};
  const GLdouble centro_base_sup[] = {0.2, 0.2, -1.5};
  const GLdouble raggio = 0.5;
  const GLdouble rgb[] = {0.0, 0.0, 1.0};

  glPointSize(1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  // glRotatef here
  drawOctagon(centro_base_inf, raggio, rgb);
  drawOctagon(centro_base_sup, raggio, rgb);
  //TODO: Draw the sides
  glFlush();
}

void initWindow() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glLoadIdentity();

  // glOrtho(view[0], view[1], view[2], view[3], view[4], view[5]);
  glFrustum(view[0], view[1], view[2], view[3], view[4], view[5]);
  glTranslatef(translate[0], translate[1], translate[2]);
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 'w':
      view[2]+=0.1;
      initWindow();
      glutPostRedisplay();
      break;
    case 's':
      view[2]-=0.1;
      initWindow();
      glutPostRedisplay();
      break;
    case 'a':
      view[0]-=0.1;
      initWindow();
      glutPostRedisplay();
      break;
    case 'd':
      view[0]+=0.1;
      initWindow();
      glutPostRedisplay();
      break;
    case 27: // ESC key
      exit(0);
  }
}

GLvoid mouse(int button, int state, int x, int y) {
  switch (button) {
    case GLUT_LEFT_BUTTON:
      if (state == GLUT_DOWN) {
        initWindow();
        glRotatef(30, 0.0, 1.0, 0.0);
        glutPostRedisplay();
      }
      break;
    case GLUT_RIGHT_BUTTON:
      if (state == GLUT_DOWN) {
        initWindow();
        glRotatef(-30, 0.0, 1.0, 0.0);
        glutPostRedisplay();
      }
      break;
  }
}

int main(int argc, char **argv) {
  const GLint size = 450;
  GLint window;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);

  glutInitWindowPosition(0, 0);
  glutInitWindowSize(size, size);
  window = glutCreateWindow("ESERCIZIO 1");

  initWindow();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMainLoop();
  return 0;
}