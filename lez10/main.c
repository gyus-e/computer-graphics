#include "utils.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

#define N 4

GLdouble g_modelview[16];
GLdouble g_projection[16];
GLint    g_viewport[4];

const Point CP0[N] = {
  {-10.0, 0.0, 0.0}, 
  {-5.0, 10.0, 0.0}, 
  {0.0, 20.0, 0.0}, 
  {5.0, 10.0, 0.0}
};
const Point CP1[N] = { 
  { -4.0, -4.0, 0.0},
  { -2.0,  4.0, 0.0},
  {  2.0, -4.0, 0.0}, 
  {  4.0,  4.0, 0.0}
};

Point CP2[N];
Point CP3[N];

const double w0[N] = {1.0, 1.0, 1.0, 1.0};
const double w1[N] ={ 1.0, 5.0, 1.0, 1.0};

unsigned int CP2_counter = 0;
unsigned int CP3_counter = 0;

Point eyePosition = {0.0, 0.0, 30.0};
const Point lookAtPoint = {0.0, 0.0, 0.0};
const Point upVector = {0.0, 1.0, 0.0};



void reshape(int width, int height) {
  if (height == 0) height = 1;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (double)width / (double)height, 0.5, 50.0);
  glMatrixMode(GL_MODELVIEW);
}



void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearDepth(1.0);
  glEnable(GL_DEPTH_TEST);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
    
  glPushMatrix();
  gluLookAt(eyePosition[X], eyePosition[Y], eyePosition[Z],
    lookAtPoint[X], lookAtPoint[Y], lookAtPoint[Z],
    upVector[X], upVector[Y], upVector[Z]);  
    glPushMatrix();
      glPointSize(2.0);
      
      glColor3fv(blue);
      bezierCurve(CP0, N, 0.0, 1.0);
      
      glColor3fv(green);
      rationalBezierCurve(CP1, w1, N);
      
      glColor3fv(red);
      if( CP2_counter == 1) {
        glBegin(GL_POINTS);
          glVertex3dv(CP2[0]);
        glEnd();
      }
      if (CP2_counter > 1 && CP2_counter < N && CP3_counter == 0) {
        rationalBezierCurve(CP2, w0, CP2_counter);
      } else if (CP2_counter == N && CP3_counter > 0 && CP3_counter <= N) {
        compositeBezierCurve(CP2, CP2_counter, w0, CP3, CP3_counter, w0);
      }
    glPopMatrix();

  glGetDoublev(GL_MODELVIEW_MATRIX, g_modelview);
  glGetDoublev(GL_PROJECTION_MATRIX, g_projection);
  glGetIntegerv(GL_VIEWPORT, g_viewport);

  glPopMatrix();
  glutSwapBuffers();
}



GLvoid mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    GLdouble refWinX, refWinY, refWinZ;
    gluProject(0.0, 0.0, 0.0, g_modelview, g_projection, g_viewport,
              &refWinX, &refWinY, &refWinZ);

    GLdouble worldX, worldY, worldZ;
    gluUnProject((GLdouble)x, (GLdouble)(g_viewport[3] - y), refWinZ,
                g_modelview, g_projection, g_viewport,
                &worldX, &worldY, &worldZ);
    
    Point mousePos;
    mousePos[X] = worldX;
    mousePos[Y] = worldY;
    mousePos[Z] = 0.0;

    if (CP2_counter < N - 1) {
      copyPoint(CP2[CP2_counter++], mousePos);
    } else if (CP2_counter == N - 1 && CP3_counter == 0) {
      copyPoint(CP2[CP2_counter++], mousePos);
      copyPoint(CP3[CP3_counter++], mousePos);
    } else if (CP3_counter > 0 && CP3_counter < N) {
      copyPoint(CP3[CP3_counter++], mousePos);
    }
    glutPostRedisplay();
  }
}


int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  (void)glutCreateWindow("Bezier Curves");
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutMouseFunc(mouse);
  glutMainLoop();
  return 0;
}
