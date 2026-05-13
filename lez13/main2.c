#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>



Point eyePosition = {5.0, 0.0, 30.0};
const Point lookAtPoint = {0.0, 0.0, 0.0};
const Point upVector = {0.0, 1.0, 0.0};



GLUnurbsObj *theNurb;

const double u1 = 0.0, u2 = 1.0, v1 = 0.0, v2 = 1.0;
const unsigned int uorder = 4, vorder = 4;
const unsigned int n = 40, m = 40;


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
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearDepth(1.0);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glPointSize(1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glShadeModel(GL_FLAT);
    
  glEnable(GL_AUTO_NORMAL);      
  glColor3dv(red);

  glPushMatrix();
    gluLookAt(eyePosition.x, eyePosition.y, eyePosition.z, lookAtPoint.x,
                lookAtPoint.y, lookAtPoint.z, upVector.x, upVector.y, upVector.z);

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


      // gluPartialDisk(partialdisk, 0.5, 1.0, 15, 10, 0.0, 270.0);
      
    
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
    eyePosition = (Point){5.0, 0.0, 30.0};
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
  (void)glutCreateWindow("ESERCIZIO 6");
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
