#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

/**
- Disegnare una sfera rossa lucida, un cono blu matto ed un toro giallo fosforescente.
(glutSolidSphere, glutSolidCone, glutSolidTorus)
- Creare due luci: una vicino alla sfera rossa e l’altra vicino al cono blu.
- Permettere all’ osservatore di ruotare gli oggetti intorno al centro della scena.
*/

#define X 0
#define Y 1
#define Z 2


const GLfloat red[4] = {1.0, 0.0, 0.0, 1.0};
const GLfloat blue[4] = {0.0, 0.0, 1.0, 1.0};
const GLfloat yellow[4] = {1.0, 1.0, 0.0, 1.0};
const GLfloat black[4] = {0.0, 0.0, 0.0, 1.0};
const GLfloat white[4] = {1.0, 1.0, 1.0, 1.0};
const GLfloat gray[4] = {0.2, 0.2, 0.2, 1.0};

const double upVector[3] = {0.0, 1.0, 0.0};
const double lookAtPoint[3] = {0.0, 0.0, 0.0};

const double step = 0.2;
const double camDistance = 30.0;
double camAngle[2] = {0.0, 0.0};
double camPosition[3] = {0.0, 0.0, camDistance};


double objAngle[3] = {0.0, 0.0, 0.0};
double objDistanceX = 4.0;

double spherePos[3] = {-4.0, 0.0, 0.0};
double conePos[3] = {4.0, 0.0, 0.0};
double torusPos[3] = {0.0, 0.0, 0.0};



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
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_NORMALIZE);
  
  glLightfv(GL_LIGHT1, GL_POSITION, (GLfloat[]){spherePos[X] + 0.5, spherePos[Y] + 0.5, spherePos[Z] +0.5, 1.0});
  glLightfv(GL_LIGHT1, GL_DIFFUSE, (GLfloat[]){1.0, 1.0, 1.0, 1.0});
  glLightfv(GL_LIGHT1, GL_SPECULAR, (GLfloat[]){1.0, 1.0, 1.0, 1.0});
  glEnable(GL_LIGHT1);
  
  glLightfv(GL_LIGHT2, GL_POSITION, (GLfloat[]){conePos[X], conePos[Y] + 2, conePos[Z], 1.0});
  glLightfv(GL_LIGHT2, GL_DIFFUSE, (GLfloat[]){1.0, 1.0, 1.0, 1.0});
  glLightfv(GL_LIGHT2, GL_SPECULAR, (GLfloat[]){1.0, 1.0, 1.0, 1.0});
  glEnable(GL_LIGHT2);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glPointSize(1.0);
  
  glPushMatrix();
    gluLookAt(camPosition[X], camPosition[Y], camPosition[Z], lookAtPoint[X],
                lookAtPoint[Y], lookAtPoint[Z], upVector[X], upVector[Y], upVector[Z]);

    glPushMatrix();
      glTranslated(spherePos[X], spherePos[Y], spherePos[Z]);
      // glColor3d(1.0, 0.0, 0.0);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);  
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, red);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, (GLfloat[]){128.0});
      glutSolidSphere(1.0, 60, 20);
    glPopMatrix();

    glPushMatrix();
      glTranslated(conePos[X], conePos[Y], conePos[Z]);
      glRotated(-90.0, 1.0, 0.0, 0.0);
      // glColor3d(0.0, 0.0, 1.0);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);        
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, gray);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, (GLfloat[]){0.0});
      glutSolidCone(1.0, 2.0, 60, 20);
    glPopMatrix();
      
    glPushMatrix();
      glTranslated(torusPos[X], torusPos[Y], torusPos[Z]);
      // glColor3d(1.0, 1.0, 0.0);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, black);        
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, black);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, yellow);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, (GLfloat[]){0.0});
      glutSolidTorus(0.5, 1.0, 60, 20);
    glPopMatrix();

  glPopMatrix();
  glutSwapBuffers();
}



void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  // case 27: // ESC
  //   exit(0);
  case 'w':
    objAngle[Y] -= step;
    break;
  case 's':
    objAngle[Y] += step;
    break;
  case 'a':
    objAngle[X] += step;
    break;
  case 'd':
    objAngle[X] -= step;
    break;
  case 'r':
    objAngle[X] = 0.0;
    objAngle[Y] = 0.0;
    break;
  }
  spherePos[X] = lookAtPoint[X] + objDistanceX * cos(objAngle[Y]) * sin(objAngle[X]);
  spherePos[Z] = lookAtPoint[Z] + objDistanceX * cos(objAngle[Y]) * cos(objAngle[X]);

  conePos[X] = lookAtPoint[X] - objDistanceX * cos(objAngle[Y]) * sin(objAngle[X]);
  conePos[Z] = lookAtPoint[Z] - objDistanceX * cos(objAngle[Y]) * cos(objAngle[X]);

  glutPostRedisplay();
}



int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowPosition(10, 10);
  glutInitWindowSize(800, 800);
  (void)glutCreateWindow("ESERCIZIO 1");
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
