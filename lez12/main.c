#include "utils.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>



Point eyePosition = {5.0, 0.0, 20.0};
const Point lookAtPoint = {0.0, 0.0, 0.0};
const Point upVector = {0.0, 1.0, 0.0};



const GLdouble ctrlpoints[4][4][3] = {
  {
    {0,  0,  0},
    {1,  0,  0.5},
    {2,  0,  1},
    {3,  0,  0}
    },
  {
    {0,  1,  0},
    {1,  1,  3},
    {2,  1,  3},
    {3,  1,  0}
    },
  {
    {0,  2,  0},
    {1,  2,  3},
    {2,  2,  3},
    {3,  2,  0}
    },
  {
    {0,  3,  0},
    {1,  3,  1},
    {2,  3,  0.5},
    {3,  3,  0}
    }
};

const double u1 = 0.0, u2 = 1.0, v1 = 0.0, v2 = 1.0;
const unsigned int uorder = 4, vorder = 4;
const double ustride = 3.0, vstride = 12.0;
const unsigned int n = 40, m = 40;



void reshape(int width, int height) {
  if (height == 0) height = 1;
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
    gluLookAt(eyePosition.x, eyePosition.y, eyePosition.z,
              lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
              upVector.x, upVector.y, upVector.z);

    glPushMatrix();
      glColor3dv(red);

      glMap2d(GL_MAP2_VERTEX_3, u1, u2, ustride, uorder, v1, v2, vstride, vorder, &ctrlpoints[0][0][0]);
      glEnable(GL_MAP2_VERTEX_3);
      glEnable(GL_AUTO_NORMAL); 

      glMapGrid2d(n, u1, u2, m, v1, v2);
      glEvalMesh2(GL_FILL, 0, n, 0, m);

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
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowPosition(10, 10);
  glutInitWindowSize(800, 800);
  (void)glutCreateWindow("BEZIER SURFACES");
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
