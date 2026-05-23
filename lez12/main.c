#include "utils.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

int displayExercise = 1;

const Point lookAtPoint = {0.0, 0.0, 0.0};
const Point upVector = {0.0, 1.0, 0.0};

const double camDistance = 20.0;
const double sensitivity = 0.01;
double camAngle[2] = {0.0, 0.0};
Point camPosition = {0.0, 0.0, camDistance};

int leftMouseDown = 0;
int rightMouseDown = 0;
int lastMouseX = 0;
int lastMouseY = 0;


const double u1 = 0.0, u2 = 1.0, v1 = 0.0, v2 = 1.0;
const unsigned int uorder = 4, vorder = 4;
const unsigned int n = 40, m = 40;

const GLdouble w[4][4] = {
    {1, 0.5, 0.5, 1}, {1, 0.5, 0.5, 1}, {1, 0.5, 0.5, 1}, {1, 0.5, 0.5, 1}
};

const GLdouble cp1[4][4][3] = {
    {{0, 0, 0}, {1, 0, 0.5}, {2, 0, 1}, {3, 0, 0}},
    {{0, 1, 0}, {1, 1, 3}, {2, 1, 3}, {3, 1, 0}},
    {{0, 2, 0}, {1, 2, 3}, {2, 2, 3}, {3, 2, 0}},
    {{0, 3, 0}, {1, 3, 1}, {2, 3, 0.5}, {3, 3, 0}}
};
GLdouble cpw1[4][4][4];

const GLfloat cp2[4][4][3] = {
    {{-3, -3, -3}, {-3, -1, -3}, {-3, 1, -3}, {-3, 3, -3}},
    {{-1, -3, -3}, {-1, -1, 3}, {-1, 1, 3}, {-1, 3, -3}},
    {{1, -3, -3}, {1, -1, 3}, {1, 1, 3}, {1, 3, -3}},
    {{3, -3, -3}, {3, -1, -3}, {3, 1, -3}, {3, 3, -3}}
};
GLfloat cpw2[4][4][4];
GLfloat knots[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
GLUnurbsObj *theNurb;




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
  glEnable(GL_AUTO_NORMAL);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glShadeModel(GL_FLAT);
  glPointSize(1.0);
  glPushMatrix();

    gluLookAt(camPosition[X], camPosition[Y], camPosition[Z], lookAtPoint[X],
              lookAtPoint[Y], lookAtPoint[Z], upVector[X], upVector[Y], upVector[Z]);

    if (displayExercise == 1) {
      glPushMatrix();
        glColor3fv(red);
        glMap2d(GL_MAP2_VERTEX_3, u1, u2, 3, uorder, v1, v2, 12, vorder, &cp1[0][0][0]);
        glEnable(GL_MAP2_VERTEX_3);

        glMap2d(GL_MAP2_VERTEX_4, u1, u2, 4, uorder, v1, v2, 16, vorder, &cpw1[0][0][0]);
        glEnable(GL_MAP2_VERTEX_4);

        glMapGrid2d(n, u1, u2, m, v1, v2);
        glEvalMesh2(GL_FILL, 0, n, 0, m);
      glPopMatrix(); 
    } else {
      glPushMatrix();
        glColor3fv(blue);
        theNurb = gluNewNurbsRenderer();
        gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
        gluBeginSurface(theNurb);
          // gluNurbsSurface(theNurb, 8, knots, 8, knots, 4*3, 3, &cp2[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
          gluNurbsSurface(theNurb, 8, knots, 8, knots, 4*4, 4, &cpw2[0][0][0], 4, 4, GL_MAP2_VERTEX_4);
        gluEndSurface(theNurb);
      glPopMatrix();
    }     

  glPopMatrix();
  glutSwapBuffers();
}


GLvoid keyboard(GLubyte key, GLint x, GLint y) {
  switch (key) {
  case 27:
    exit(0);
    break;
  case '1':
    displayExercise = 1;
    break;
  case '2':
    displayExercise = 2;
    break;
  }
  glutPostRedisplay();
}


GLvoid mouse(GLint button, GLint state, GLint x, GLint y) {
  switch (button) {
  case GLUT_LEFT_BUTTON:
    leftMouseDown = (state == GLUT_DOWN);
  case GLUT_RIGHT_BUTTON:
    rightMouseDown = (state == GLUT_DOWN);
  default:
    lastMouseX = x;
    lastMouseY = y;
  }
}



GLvoid motion(GLint x, GLint y) {
  int dx = x - lastMouseX;
  int dy = y - lastMouseY;
  lastMouseX = x;
  lastMouseY = y;
  if (leftMouseDown) { 
    camAngle[Y] += dy * sensitivity;
    camAngle[X] -= dx * sensitivity;
  }
  camPosition[Y] = lookAtPoint[Y] + camDistance * sin(camAngle[Y]);
  camPosition[X] = lookAtPoint[X] + camDistance * cos(camAngle[Y]) * sin(camAngle[X]);
  camPosition[Z] = lookAtPoint[Z] + camDistance * cos(camAngle[Y]) * cos(camAngle[X]);
  glutPostRedisplay();
}



int main(int argc, char **argv) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      cpw1[i][j][X] = cp1[i][j][X] * w[i][j];
      cpw1[i][j][Y] = cp1[i][j][Y] * w[i][j];
      cpw1[i][j][Z] = cp1[i][j][Z] * w[i][j];
      cpw1[i][j][3] = w[i][j];
    }
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      cpw2[i][j][X] = cp2[i][j][X] * w[i][j];
      cpw2[i][j][Y] = cp2[i][j][Y] * w[i][j];
      cpw2[i][j][Z] = cp2[i][j][Z] * w[i][j];
      cpw2[i][j][3] = w[i][j];
    }
  }

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowPosition(10, 10);
  glutInitWindowSize(800, 800);
  (void)glutCreateWindow("SURFACES");
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutMainLoop();
  return 0;
}
