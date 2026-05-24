#include "utils.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>



GLUnurbsObj *theNurb, *theNurb2, *theNurb3;



const unsigned int deg = 3;
const unsigned int order = deg + 1;
const unsigned int numPoints = 4;
const unsigned int numKnots = numPoints + order;
GLfloat cp[4 * 3] = {
    -4.0, -4.0, 0.0,
    -2.0, 4.0, 0.0,
    2.0, -4.0, 0.0,
    4.0, 4.0, 0.0
};
float knots[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
float w[4] = {1.0, 5.0, 1.0, 1.0};
GLfloat cpw[4 * 4];



const unsigned int circleDeg = 2;
const unsigned int circleOrder = circleDeg + 1;
const unsigned int circleNumPoints = 7;
const unsigned int circleNumKnots = circleNumPoints + circleOrder;
GLfloat circleCP[7 * 4] = {
    1.0, 2.0, 0.0, 0.0,
    1.0, 3.0, 0.0, 0.0,
    3.0, 3.0, 0.0, 0.0,
    3.0, 2.0, 0.0, 0.0,
    3.0, 1.0, 0.0, 0.0,
    1.0, 1.0, 0.0, 0.0,
    1.0, 2.0, 0.0, 0.0
};
float circleKnots[10] = {0.0, 0.0, 0.0, 0.25, 0.5, 0.5, 0.75, 1.0, 1.0, 1.0};
float circleW[7] = {1.0, 0.5, 0.5, 1.0, 0.5, 0.5, 1.0};
GLfloat circleCpw[7 * 4];



Point camPosition = {0.0, 0.0, 10.0};
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
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glPushMatrix();
    gluLookAt(camPosition[X], camPosition[Y], camPosition[Z],
              lookAtPoint[X], lookAtPoint[Y], lookAtPoint[Z],
              upVector[X], upVector[Y], upVector[Z]);
    
    glPointSize(1.0);

    glPushMatrix();
      glColor3fv(red);
      theNurb = gluNewNurbsRenderer();
      gluNurbsCallback(theNurb, GLU_ERROR, (GLvoid (*))nurbsError);
      gluNurbsProperty(theNurb, GLU_U_STEP, 360);
      gluNurbsProperty(theNurb, GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
      gluBeginCurve(theNurb);
        // 3d points: stride = 3
        gluNurbsCurve(theNurb, numKnots, knots, 3, &cp[0], order, GL_MAP1_VERTEX_3); 
      gluEndCurve(theNurb);
    glPopMatrix();

    glPushMatrix();
      glColor3fv(green);
      theNurb2 = gluNewNurbsRenderer();
      gluNurbsCallback(theNurb2, GLU_ERROR, (GLvoid (*))nurbsError);
      gluNurbsProperty(theNurb2, GLU_U_STEP, 360);
      gluNurbsProperty(theNurb2, GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
      gluBeginCurve(theNurb2);
        // homogenized points: stride = 4
        gluNurbsCurve(theNurb2, numKnots, knots, 4, &cpw[0], order, GL_MAP1_VERTEX_4); 
      gluEndCurve(theNurb2);
    glPopMatrix();

    glPushMatrix();
      glColor3fv(blue);
      theNurb3 = gluNewNurbsRenderer();
      gluNurbsCallback(theNurb3, GLU_ERROR, (GLvoid (*))nurbsError);
      gluNurbsProperty(theNurb3, GLU_U_STEP, 360);
      gluNurbsProperty(theNurb3, GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
      gluBeginCurve(theNurb3);
        gluNurbsCurve(theNurb3, circleNumKnots, circleKnots, 4, &circleCpw[0], circleOrder, GL_MAP1_VERTEX_4);
      gluEndCurve(theNurb3);
    glPopMatrix();

  glPopMatrix();
  glutSwapBuffers();
}

int main(int argc, char **argv) {
  omogenizePoints(cpw, numPoints, 3, cp, w);
  omogenizePoints(circleCpw, circleNumPoints, 4, circleCP, circleW);

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
