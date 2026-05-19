#include "utils.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>



Point eyePosition = {5.0, 0.0, 20.0};
const Point lookAtPoint = {0.0, 0.0, 0.0};
const Point upVector = {0.0, 1.0, 0.0};



GLfloat cp[4][4][3] =
    {{{-3, -3, -3}, {-3, -1, -3}, {-3, 1, -3}, {-3, 3, -3}},
     {{-1, -3, -3}, {-1, -1, 3}, {-1, 1, 3}, {-1, 3, -3}},
     {{1, -3, -3}, {1, -1, 3}, {1, 1, 3}, {1, 3, -3}},
     {{3, -3, -3}, {3, -1, -3}, {3, 1, -3}, {3, 3, -3}}};

GLfloat knots[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};

const GLfloat w[4][4] = {{1, .5, .5, 1}, {1, .5, .5, 1}, {1, .5, .5, 1}, {1, .5, .5, 1}};

GLfloat cpw[4][4][4];

GLUnurbsObj *theNurb;

const double u1 = 0.0, u2 = 1.0, v1 = 0.0, v2 = 1.0;
const unsigned int uorder = 4, vorder = 4;
const unsigned int n = 40, m = 40;



void reshape(int width, int height) {
  if (height == 0)
    height = 1;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(20.0, (double)width / (double)height, 10.0, 50.0);
  glMatrixMode(GL_MODELVIEW);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearDepth(1.0);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glPointSize(1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glShadeModel(GL_FLAT);

  glPushMatrix();
  gluLookAt(eyePosition.x, eyePosition.y, eyePosition.z, lookAtPoint.x,
            lookAtPoint.y, lookAtPoint.z, upVector.x, upVector.y, upVector.z);

    glPushMatrix();
      glColor3dv(red);
      glEnable(GL_AUTO_NORMAL);
      
      theNurb = gluNewNurbsRenderer();
      gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
      gluBeginSurface(theNurb);
        // gluNurbsSurface(theNurb, 8, knots, 8, knots, 4*3, 3, &cp[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
        gluNurbsSurface(theNurb, 8, knots, 8, knots, 4*4, 4, &cpw[0][0][0], 4, 4, GL_MAP2_VERTEX_4);
      gluEndSurface(theNurb);

    glPopMatrix();
  glPopMatrix();
  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 27: // ESC
    exit(0);
    break;
  case 'w':
    eyePosition.y += 0.5;
    glutPostRedisplay();
    break;
  case 's':
    eyePosition.y -= 0.5;
    glutPostRedisplay();
    break;
  case 'a':
    eyePosition.x -= 0.5;
    glutPostRedisplay();
    break;
  case 'd':
    eyePosition.x += 0.5;
    glutPostRedisplay();
    break;
  case 'r':
    eyePosition = (Point){5.0, 0.0, 20.0};
    glutPostRedisplay();
    break;
  default:
    break;
  }
}

int main(int argc, char **argv) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      cpw[i][j][X] = cp[i][j][X] * w[i][j];
      cpw[i][j][Y] = cp[i][j][Y] * w[i][j];
      cpw[i][j][Z] = cp[i][j][Z] * w[i][j];
      cpw[i][j][3] = w[i][j];
    }
  }

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowPosition(10, 10);
  glutInitWindowSize(800, 800);
  (void)glutCreateWindow("NURBS SURFACES");
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
