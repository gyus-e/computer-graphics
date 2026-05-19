#include "utils.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>



GLUnurbsObj *theNurb;


float cp[4][3] = {
    {-4.0, -4.0, 0.0},
    {-2.0, 4.0, 0.0},
    {2.0, -4.0, 0.0},
    {4.0, 4.0, 0.0}
};
float w[4] = {1.0, 5.0, 1.0, 1.0};
float knots[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
float cpw[4][4];

float circleCP[7][3] = {
    {1.0, 2.0, 0.0},
    {1.0, 3.0, 0.0},
    {3.0, 3.0, 0.0},
    {3.0, 2.0, 0.0},
    {3.0, 1.0, 0.0},
    {1.0, 1.0, 0.0},
    {1.0, 2.0, 0.0}
};
float circleW[7] = {1.0, 0.5, 0.5, 1.0, 0.5, 0.5, 1.0};
float circleKnots[10] = {0.0, 0.0, 0.0, 0.25, 0.5, 0.5, 0.75, 1.0, 1.0, 1.0};
float circleCpw[7][4];


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
      theNurb = gluNewNurbsRenderer();
      gluNurbsCallback(theNurb, GLU_ERROR, (GLvoid (*))nurbsError);
      gluNurbsProperty(theNurb, GLU_U_STEP, 50);
      gluNurbsProperty(theNurb, GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
      gluBeginCurve(theNurb);
        // gluNurbsCurve(theNurb, 8, knots, 3, &cp[0][0], 4, GL_MAP1_VERTEX_3);
        // gluNurbsCurve(theNurb, 8, knots, 4, &cpw[0][0], 4, GL_MAP1_VERTEX_4);
        gluNurbsCurve(theNurb, 10, circleKnots, 4, &circleCpw[0][0], 3, GL_MAP1_VERTEX_4);
      gluEndCurve(theNurb);
    glPopMatrix();
  glPopMatrix();
  glutSwapBuffers();
}


void omogenizePoints(double cpw[][4], const Point cp[], const size_t numPoints, const double w[]) {
  for (int i = 0; i < numPoints; i++) {
    (void)omogenize3dp(cpw[i], &cp[i], w[i]);
  }
}

void omogenizePoints2(double cpw[][4], const double cp[][3], const size_t numPoints, const double w[]) {
  for (int i = 0; i < numPoints; i++) {
    (void)omogenize3dv(cpw[i], cp[i], w[i]);
  }
}

int main(int argc, char **argv) {
  omogenizePoints2((double**)cpw, (double**)cp, 4, (double*)w);
  omogenizePoints2((double**)circleCpw, (double**)circleCP, 7, (double*)circleW);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(500, 500);
  (void)glutCreateWindow("NURBS");
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}
