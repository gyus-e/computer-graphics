#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

/**
Disegnare due sfere che si intersecano. 
Permettere all’ osservatore di ruotare le sfere intorno al centro della scena.
Creare due luci:
– Una luce bianca in  prossimità delle sfere e  che si muova con le sfere.
– Una luce rossa che rimane coerente alla posizione dell’ osservatore.
*/

const Point upVector = {0.0, 1.0, 0.0};
const Point lookAtPoint = {0.0, 0.0, 0.0};

const double camSpeed = 0.5;
const double camDistance = 30.0;
double camAngle[2] = {0.0, 0.0};
Point camPosition = {0.0, 0.0, camDistance};

const double sphereDistance = 0.5;
double sphereAngle[2] = {0.0, 0.0};
double c1[3] = {-0.5, 0.0, 0.0}; 
double c2[3] = {0.5, 0.0, 0.0};
GLUquadricObj *s1, *s2;



void errorCallback(GLenum errorCode) {
  const GLubyte *estring;
  estring = gluErrorString(errorCode);
  fprintf(stderr, "Nurbs Error: %s\n", estring);
  exit(0);
}



void reshape(int width, int height) {
  if (height == 0)
    height = 1;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(20.0, (double)width / (double)height, 20.0, 100.0);
  glMatrixMode(GL_MODELVIEW);
}



void display() {
  glClearDepth(1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_NORMALIZE);
  
  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT1, GL_POSITION, (GLfloat[]){camPosition.x, camPosition.y, camPosition.z, 1.0});
  glLightfv(GL_LIGHT1, GL_DIFFUSE, (GLfloat[]){1.0, 0.0, 0.0, 1.0});

  glEnable(GL_LIGHT2);
  glLightfv(GL_LIGHT2, GL_POSITION, (GLfloat[]){(c1[X] + c2[X]) * 0.5, (c1[Y] + c2[Y]) * 0.5 + 5, (c1[Z] + c2[Z]) * 0.5 - 5, 1.0});
  glLightfv(GL_LIGHT2, GL_DIFFUSE, (GLfloat[]){1.0, 1.0, 1.0, 1.0});

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glPointSize(1.0);
  glColor3dv(white);
  
  glPushMatrix();
    gluLookAt(camPosition.x, camPosition.y, camPosition.z, lookAtPoint.x,
                lookAtPoint.y, lookAtPoint.z, upVector.x, upVector.y, upVector.z);

    glPushMatrix();
      glTranslated(c1[X], c1[Y], c1[Z]);
      s1 = gluNewQuadric();
      gluQuadricCallback(s1, GLU_ERROR, (GLvoid(*))errorCallback);
      gluQuadricDrawStyle(s1, GLU_FILL); 
      gluQuadricOrientation(s1, GLU_OUTSIDE); 
      gluQuadricNormals(s1, GLU_FLAT);
      gluSphere(s1, 1.0, 60, 20);
    glPopMatrix();

    glPushMatrix();
      glTranslated(c2[X], c2[Y], c2[Z]);
      s2 = gluNewQuadric();
      gluQuadricCallback(s2, GLU_ERROR, (GLvoid(*))errorCallback);
      gluQuadricDrawStyle(s2, GLU_FILL); 
      gluQuadricOrientation(s2, GLU_OUTSIDE); 
      gluQuadricNormals(s2, GLU_FLAT);
      gluSphere(s2, 1.0, 60, 20);
    glPopMatrix();
      
  glPopMatrix();
  glutSwapBuffers();
}



void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  // case 27: // ESC
  //   exit(0);
  case 'w':
    camAngle[Y] -= camSpeed;
    break;
  case 's':
    camAngle[Y] += camSpeed;
    break;
  case 'a':
    camAngle[X] += camSpeed;
    break;
  case 'd':
    camAngle[X] -= camSpeed;
    break;
  case 'q':
    sphereAngle[Y] -= camSpeed;
    break;
  case 'e':
    sphereAngle[Y] += camSpeed;
    break;
  case 'r':
    camAngle[X] = 0.0;
    camAngle[Y] = 0.0;
    sphereAngle[X] = 0.0;
    sphereAngle[Y] = 0.0;
    break;
  }
  camPosition.y = lookAtPoint.y + camDistance * sin(camAngle[Y]);
  camPosition.x = lookAtPoint.x + camDistance * cos(camAngle[Y]) * sin(camAngle[X]);
  camPosition.z = lookAtPoint.z + camDistance * cos(camAngle[Y]) * cos(camAngle[X]);

  c1[X] = -sphereDistance * cos(sphereAngle[Y]) * sin(sphereAngle[X]);
  c1[Y] = sphereDistance * sin(sphereAngle[Y]);
  c1[Z] = sphereDistance * cos(sphereAngle[Y]) * cos(sphereAngle[X]);

  c2[X] = sphereDistance * cos(sphereAngle[Y]) * sin(sphereAngle[X]);
  c2[Y] = -sphereDistance * sin(sphereAngle[Y]);
  c2[Z] = -sphereDistance * cos(sphereAngle[Y]) * cos(sphereAngle[X]);
  glutPostRedisplay();
}



int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowPosition(10, 10);
  glutInitWindowSize(800, 800);
  (void)glutCreateWindow("ESERCIZIO 6");
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
