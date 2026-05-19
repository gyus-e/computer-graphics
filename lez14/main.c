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
const double radius = 30.0;
const double camSpeed = 0.5;
double camAngle[2] = {0.0, 0.0};
Point eyePosition = {0.0, 0.0, radius};
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
  
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_POSITION, (GLfloat[]){lookAtPoint.x, lookAtPoint.y + 3, lookAtPoint.z, 1.0});
  glLightfv(GL_LIGHT0, GL_DIFFUSE, (GLfloat[]){1.0, 1.0, 1.0, 1.0});
  
  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT1, GL_POSITION, (GLfloat[]){eyePosition.x, eyePosition.y + 3, eyePosition.z, 1.0});
  glLightfv(GL_LIGHT1, GL_DIFFUSE, (GLfloat[]){1.0, 0.0, 0.0, 1.0});

  glPointSize(1.0);
  glColor3dv(red);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();   

  glPushMatrix();
    gluLookAt(eyePosition.x, eyePosition.y, eyePosition.z, lookAtPoint.x,
                lookAtPoint.y, lookAtPoint.z, upVector.x, upVector.y, upVector.z);

    glPushMatrix();
      glTranslated(-0.4, 0.0, 0.0);
      s1 = gluNewQuadric();
      gluQuadricCallback(s1, GLU_ERROR, (GLvoid(*))errorCallback);
      gluQuadricDrawStyle(s1, GLU_FILL); 
      gluQuadricOrientation(s1, GLU_OUTSIDE); 
      gluQuadricNormals(s1, GLU_FLAT);
      gluSphere(s1, 0.75, 30, 10);
    glPopMatrix();

    glPushMatrix();
      glTranslated(0.4, 0.0, 0.0);
      s2 = gluNewQuadric();
      gluQuadricCallback(s2, GLU_ERROR, (GLvoid(*))errorCallback);
      gluQuadricDrawStyle(s2, GLU_FILL); 
      gluQuadricOrientation(s2, GLU_OUTSIDE); 
      gluQuadricNormals(s2, GLU_FLAT);
      gluSphere(s2, 0.75, 30, 10);
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
  case 'r':
    camAngle[X] = 0.0;
    camAngle[Y] = 0.0;
    break;
  }
  eyePosition.y = lookAtPoint.y + radius * sin(camAngle[Y]);
  eyePosition.x = lookAtPoint.x + radius * cos(camAngle[Y]) * sin(camAngle[X]);
  eyePosition.z = lookAtPoint.z + radius * cos(camAngle[Y]) * cos(camAngle[X]);
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
