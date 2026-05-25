#include <math.h>
#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"
#include <GL/freeglut_std.h>


enum {X, Y, Z};

const double upVector[3] = {0.0, 1.0, 0.0};
const double lookAtPoint[3] = {0.0, 0.0, 0.0};

const double camDistance = 50.0;
double camAngle[2] = {0.0, 0.0};
double camPosition[3] = {0.0, 0.0, camDistance};

const double sensitivity = 0.02;
int leftMouseDown = 0;
int rightMouseDown = 0;
int lastMouseX = 0;
int lastMouseY = 0;

double baseY = -3.0;
double baseHeight = 1.0;
double maxHeight = 6.0;

unsigned int currentLight = 0;



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
  glClearDepth(1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHT0);


  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glPointSize(1.0);
  
  glPushMatrix();
    gluLookAt(camPosition[X], camPosition[Y], camPosition[Z], lookAtPoint[X],
                lookAtPoint[Y], lookAtPoint[Z], upVector[X], upVector[Y], upVector[Z]);
    
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3d(0.1, 0.01, 0.0);
    glPushMatrix();      
      glTranslated(0.0, baseY, 0.0);
      glRotated(-90.0, 1.0, 0.0, 0.0);
      GLUquadricObj *base = gluNewQuadric();
      gluQuadricDrawStyle(base, GLU_FILL); 
      gluQuadricOrientation(base, GLU_OUTSIDE);
      gluQuadricNormals(base, GLU_FLAT);
      gluCylinder(base, 0.5, 0.5, baseHeight, 30, 10);
    glPopMatrix();

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3d(0.0, 0.3, 0.0);
    for (double h = baseY + baseHeight; h <= maxHeight; h+=0.5) {
      glPushMatrix();      
        glTranslated(0.0, h, 0.0);
        glRotated(-90.0, 1.0, 0.0, 0.0);
        GLUquadricObj *cylinder = gluNewQuadric();
        gluQuadricDrawStyle(cylinder, GLU_FILL); 
        gluQuadricOrientation(cylinder, GLU_OUTSIDE);
        gluQuadricNormals(cylinder, GLU_FLAT);
        gluCylinder(cylinder, maxHeight - h + 1, 0.1, 1.0, 30, 10);
      glPopMatrix();
    }

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColorMaterial(GL_FRONT_AND_BACK, GL_SHININESS);
    glColor3d(1.0, 1.0, 0.0);
    for (int i = 0; i < 2; i++) {
      glPushMatrix();
        glTranslated(0.0, maxHeight + 1.0 + i*0.2, 0.0);
        glRotated(i*180+35.0, 0.0, 0.0, 1.0);
        glScaled(0.6, 0.6, 0.6);
        glutSolidTetrahedron();
      glPopMatrix();
    }

  glPopMatrix();
  glutSwapBuffers();
}



void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 27: // ESC
    exit(0);
    break;
  default:
    return;
  }
  glutPostRedisplay();
}



GLvoid mouse(GLint button, GLint state, GLint x, GLint y) {
  switch (button) {
  case GLUT_LEFT_BUTTON:
    leftMouseDown = (state == GLUT_DOWN);
    break;
  case GLUT_RIGHT_BUTTON:
    rightMouseDown = (state == GLUT_DOWN);
    break;
  default:
    lastMouseX = x;
    lastMouseY = y;
    break;
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
  if (camAngle[Y] > 0.5) camAngle[Y] = 0.5;
  if (camAngle[Y] < -0.5) camAngle[Y] = -0.5;
  camPosition[Y] = lookAtPoint[Y] + camDistance * sin(camAngle[Y]);
  camPosition[X] = lookAtPoint[X] + camDistance * cos(camAngle[Y]) * sin(camAngle[X]);
  camPosition[Z] = lookAtPoint[Z] + camDistance * cos(camAngle[Y]) * cos(camAngle[X]);
  glutPostRedisplay();
}



GLvoid timer(GLint value) {
  //...
  glutPostRedisplay();
  glutTimerFunc(1000, timer, 0);
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
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutTimerFunc(1000, timer, 0);
  glutMainLoop();
  return 0;
}
