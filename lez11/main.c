#include "utils.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>



GLUnurbsObj *theNurb;


GLfloat cp[4][3] = {
    {-4.0, -4.0, 0.0},
    {-2.0, 4.0, 0.0},
    {2.0, -4.0, 0.0},
    {4.0, 4.0, 0.0}
};
GLfloat w[4] = {1.0, 5.0, 1.0, 1.0};
GLfloat knots[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
GLfloat cpw[4][4];

GLfloat circleControlPoints[7][3] = {
    {1.0, 2.0, 0.0},
    {-1.0, 3.0, 0.0},
    {3.0, 3.0, 0.0},
    {3.0, 2.0, 0.0},
    {3.0, 1.0, 0.0},
    {1.0, 1.0, 0.0},
    {1.0, 2.0, 0.0}
};
GLfloat circleW[7] = {1.0, 0.5, 0.5, 1.0, 0.5, 0.5, 1.0};
GLfloat circleKnots[10] = {0.0, 0.0, 0.0, 0.25, 0.5, 0.5, 0.75, 1.0, 1.0, 1.0};
GLfloat circleCpw[7][4];


Point eyePosition = {0.0, 0.0, 10.0};
const Point lookAtPoint = {0.0, 0.0, 0.0};
const Point upVector = {0.0, 1.0, 0.0};



void reshape(int width, int height) {
  if (height == 0) height = 1;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (double)width / (double)height, 0.1, 50.0);
  glMatrixMode(GL_MODELVIEW);
}



void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearDepth(1.0);
  glEnable(GL_DEPTH_TEST);
  
//   glEnable(GL_LIGHTING);
//   glEnable(GL_LIGHT0);
  
  glPointSize(1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
//   glShadeModel(GL_FLAT);

  glPushMatrix();
    gluLookAt(eyePosition.x, eyePosition.y, eyePosition.z,
              lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
              upVector.x, upVector.y, upVector.z);

    glPushMatrix();
      glColor3d(1.0, 1.0, 1.0);
      theNurb = gluNewNurbsRenderer();
      gluBeginCurve(theNurb);
        // gluNurbsCurve(theNurb, 8, knots, 3, &cp[0][0], 4, GL_MAP1_VERTEX_3);
        // gluNurbsCurve(theNurb, 8, knots, 4, &cpw[0][0], 4, GL_MAP1_VERTEX_4);
        gluNurbsCurve(theNurb, 10, circleKnots, 4, &circleCpw[0][0], 3, GL_MAP1_VERTEX_4);
      gluEndCurve(theNurb);
    glPopMatrix();
  glPopMatrix();
  glFlush();
//   glutSwapBuffers();
}




int main(int argc, char **argv) {
  for (int i = 0; i < 4; i++) {
    cpw[i][0] = cp[i][0] * w[i];
    cpw[i][1] = cp[i][1] * w[i];
    cpw[i][2] = cp[i][2] * w[i];
    cpw[i][3] = w[i];
  }

  for (int i = 0; i < 7; i++) {
    circleCpw[i][0] = circleControlPoints[i][0] * circleW[i];
    circleCpw[i][1] = circleControlPoints[i][1] * circleW[i];
    circleCpw[i][2] = circleControlPoints[i][2] * circleW[i];
    circleCpw[i][3] = circleW[i];
  }

  glutInit(&argc, argv);
//   glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(500, 500);
  (void)glutCreateWindow("NURBS");
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
//   glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
