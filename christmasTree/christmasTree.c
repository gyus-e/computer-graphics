#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"
#include <GL/freeglut_std.h>


#define X 0
#define Y 1
#define Z 2


double eyePosition[3] = {0.0, 5.0, 50.0};
const double lookAtPoint[3] = {0.0, 0.0, 0.0};
const double upVector[3] = {0.0, 1.0, 0.0};



void reshape(int width, int height) {
  if (height == 0)
    height = 1;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(20.0, (double)width / (double)height, 20.0, 100.0);
  glMatrixMode(GL_MODELVIEW);
}



void display(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearDepth(1.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_AUTO_NORMAL);
  glShadeModel(GL_FLAT);
  
  glPointSize(1.0);
  glColor3d(0.0, 0.0, 0.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glPushMatrix();
    gluLookAt(eyePosition[X], eyePosition[Y], eyePosition[Z], lookAtPoint[X],
                lookAtPoint[Y], lookAtPoint[Z], upVector[X], upVector[Y], upVector[Z]);


    glPushMatrix();      
      glTranslated(0.0, 0.0, 0.0);
      glRotated(-90.0, 1.0, 0.0, 0.0);

      GLUquadricObj *base = gluNewQuadric();
      gluQuadricDrawStyle(base, GLU_FILL); 
      gluQuadricOrientation(base, GLU_OUTSIDE);
      gluQuadricNormals(base, GLU_FLAT);
      gluCylinder(base, 0.5, 0.5, 1.0, 30, 10);
    glPopMatrix();

    for (double h = 1.0; h <= 5.0; h+=0.5) {
      glPushMatrix();      
        glTranslated(0.0, h, 0.0);
        glRotated(-90.0, 1.0, 0.0, 0.0);

        GLUquadricObj *cylinder = gluNewQuadric();
        gluQuadricDrawStyle(cylinder, GLU_FILL); 
        gluQuadricOrientation(cylinder, GLU_OUTSIDE);
        gluQuadricNormals(cylinder, GLU_FLAT);
        gluCylinder(cylinder, 5.0 - h + 1, 0.1, 1.0, 30, 10);
      glPopMatrix();
    }

  glPopMatrix();
  glutSwapBuffers();
}



int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(10, 10);
  glutInitWindowSize(800, 800);
  (void)glutCreateWindow("Christmas Tree");
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}
