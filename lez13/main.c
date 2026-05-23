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


GLfloat E[5][2]={{0,0},{1,0},{1,1},{0,1},{0,0}};

GLfloat PP[5][2]={
  {0.3,0.3},{0.3,0.7},{0.7,0.7},{0.7,0.3},{0.3,0.3}
};

GLfloat PN[7][2]={
  {0.3,0.5},{0.3,0.8},{0.6,0.8},{0.6,0.5},{0.6, 0.2},{0.3, 0.2},{0.3, 0.5}
};

GLfloat PNW[7][4];

GLfloat knots_trim[12] = {0.0, 0.0, 0.0, 0.0, 0.25, 0.5, 0.75, 1.0, 1.0, 1.0, 1.0, 1.0};


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
  gluPerspective(20.0, (double)width / (double)height, 10.0, 80.0);
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

  glPushMatrix();
    gluLookAt(camPosition[X], camPosition[Y], camPosition[Z], lookAtPoint[X],
                lookAtPoint[Y], lookAtPoint[Z], upVector[X], upVector[Y], upVector[Z]);

    glPointSize(1.0);
    glColor3fv(red);

      if (displayExercise == 1) {
        glPushMatrix();
          theNurb = gluNewNurbsRenderer();
          gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
          gluBeginSurface(theNurb);
            gluNurbsSurface(theNurb, 8, knots, 8, knots, 4*3, 3, &cp[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
            
            gluBeginTrim(theNurb);
              gluPwlCurve(theNurb, 5, &E[0][0], 2, GLU_MAP1_TRIM_2);
            gluEndTrim(theNurb);

            gluBeginTrim(theNurb);
              // gluPwlCurve(theNurb, 5, &PP[0][0], 2, GLU_MAP1_TRIM_2);
              gluNurbsCurve(theNurb, 11, knots_trim, 2, &PN[0][0], 4, GLU_MAP1_TRIM_2);
            gluEndTrim(theNurb);     
          gluEndSurface(theNurb);
        glPopMatrix();
        
      } else if (displayExercise == 2) {
        glPushMatrix();
          glTranslated(0.0, 0.0, 0.0);
          GLUquadricObj *sphere = gluNewQuadric();
          gluQuadricCallback(sphere, GLU_ERROR, (GLvoid(*))errorCallback);
          gluQuadricDrawStyle(sphere, GLU_FILL); 
          gluQuadricOrientation(sphere, GLU_OUTSIDE); 
          gluQuadricNormals(sphere, GLU_FLAT);
          gluSphere(sphere, 0.75, 15, 10);
        glPopMatrix();

        glPushMatrix();
          glTranslated(2.0, 0.0, 0.0);
          GLUquadricObj *cylinder = gluNewQuadric();
          gluQuadricCallback(cylinder, GLU_ERROR, (GLvoid(*))errorCallback);
          gluQuadricDrawStyle(cylinder, GLU_FILL); 
          gluQuadricOrientation(cylinder, GLU_OUTSIDE); 
          gluCylinder(cylinder, 0.75, 0.75, 2.0, 15, 10);
        glPopMatrix();

        glPushMatrix();
          glTranslated(-2.0, 0.0, 0.0);
          GLUquadricObj *disk = gluNewQuadric();
          gluQuadricCallback(disk, GLU_ERROR, (GLvoid(*))errorCallback);
          gluQuadricDrawStyle(disk, GLU_FILL); 
          gluQuadricOrientation(disk, GLU_OUTSIDE); 
          gluDisk(disk, 0.5, 1.0, 15, 10);
        glPopMatrix();
      }
  glPopMatrix();
  glutSwapBuffers();
}


GLvoid keyboard(GLubyte key, GLint x, GLint y) {
  switch (key) {
  case '1':
    displayExercise = 1;
    break;
  case '2':
    displayExercise = 2;
    break;
  default:
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
      cpw[i][j][X] = cp[i][j][X] * w[i][j];
      cpw[i][j][Y] = cp[i][j][Y] * w[i][j];
      cpw[i][j][Z] = cp[i][j][Z] * w[i][j];
      cpw[i][j][3] = w[i][j];
    }
  }

  for(int i = 0; i < 7; i++) {
    for(int j = 0; j < 4; j++) {
        PNW[i][X] = PN[i][X] * 1;
        PNW[i][Y] = PN[i][Y] * 1;
        PNW[i][Z] = 0 * 1;
        PNW[i][3] = 1;
    }
  }

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowPosition(10, 10);
  glutInitWindowSize(800, 800);
  (void)glutCreateWindow("B-SPLINE SURFACES");
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutMainLoop();
  return 0;
}
