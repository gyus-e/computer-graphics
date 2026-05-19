#include "utils.h"
#include <math.h>
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


const Point upVector = {0.0, 1.0, 0.0};
const Point lookAtPoint = {0.0, 0.0, 0.0};

const double camSpeed = 0.1;
const double camDistance = 30.0;
double camAngle[2] = {0.0, 0.0};
Point camPosition = {0.0, 0.0, camDistance};

int lights_on[3] = {0, 0, 0};


GLfloat E[5][2]={{0,0},{1,0},{1,1},{0,1},{0,0}};

GLfloat PP[5][2]={{0.3,0.3},{0.3,0.7},
{0.7,0.7},{0.7,0.3},{0.3,0.3}};

GLfloat PN[7][2]={{0.3,0.5},{0.3,0.8},
{0.6,0.8},{0.6,0.5},{0.6, 0.2},
{0.3, 0.2},{0.3, 0.5}};

GLfloat PNW[7][4];

GLfloat knots_trim[12] = {0.0, 0.0, 0.0, 0.0, 0.25, 0.5, 0.75, 1.0, 1.0, 1.0, 1.0, 1.0};


GLfloat cp[4][4][3] =
    {{{-3, -3, -3}, {-3, -1, -3}, {-3, 1, -3}, {-3, 3, -3}},
     {{-1, -3, -3}, {-1, -1, 3}, {-1, 1, 3}, {-1, 3, -3}},
     {{1, -3, -3}, {1, -1, 3}, {1, 1, 3}, {1, 3, -3}},
     {{3, -3, -3}, {3, -1, -3}, {3, 1, -3}, {3, 3, -3}}};

GLfloat knots[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};

const GLfloat w[4][4] = {{1, .5, .5, 1}, {1, .5, .5, 1}, {1, .5, .5, 1}, {1, .5, .5, 1}};

GLfloat cpw[4][4][4];

GLUnurbsObj *theNurb;

const double u1 = 0.0, u2 = 1.0, v1 = 0.0, v2 = 1.0;
const unsigned int uorder = 4, vorder = 4;
const unsigned int n = 40, m = 40;




void reshape(int width, int height) {
  if (height == 0)
    height = 1;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(20.0, (double)width / (double)height, 10.0, 80.0);
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

  //Una luce ambientale persistente anche con le luci spente
  glEnable(GL_LIGHT0);
  
  // Tre luci che si possano accendere alternativamente di tre colori diversi.
  glLightfv(GL_LIGHT1, GL_DIFFUSE, (GLfloat[]){1.0, 0.0, 0.0, 1.0});
  glLightfv(GL_LIGHT2, GL_DIFFUSE, (GLfloat[]){0.0, 1.0, 0.0, 1.0});
  glLightfv(GL_LIGHT3, GL_DIFFUSE, (GLfloat[]){0.0, 0.0, 1.0, 1.0});
  
  // L’illuminazione anche delle facce posteriori.
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glPointSize(1.0);
  glShadeModel(GL_FLAT);

  glPushMatrix();
    gluLookAt(camPosition.x, camPosition.y, camPosition.z, lookAtPoint.x,
                lookAtPoint.y, lookAtPoint.z, upVector.x, upVector.y, upVector.z);

    glPushMatrix();
      glColor3dv(red);
      glEnable(GL_AUTO_NORMAL);
      
      theNurb = gluNewNurbsRenderer();
      gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
      
      gluBeginSurface(theNurb);
        
        gluNurbsSurface(theNurb, 8, knots, 8, knots, 4*3, 3, &cp[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
        
        gluBeginTrim(theNurb);
            gluPwlCurve(theNurb, 5, &E[0][0], 2, GLU_MAP1_TRIM_2);
        gluEndTrim(theNurb);
        
        gluBeginTrim(theNurb);
            gluPwlCurve(theNurb, 5, &PP[0][0], 2, GLU_MAP1_TRIM_2);
            // gluNurbsCurve(theNurb, 12, knots_trim, 2, &PN[0][0], 4, GLU_MAP1_TRIM_2);
        gluEndTrim(theNurb);    
        
      
      gluEndSurface(theNurb);

    glPopMatrix();
    
  glPopMatrix();
  glutSwapBuffers();
}



void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  // case 27: // ESC
  //   exit(0);
  case '1':
    if (lights_on[0]) {
        glDisable(GL_LIGHT1);
        lights_on[0] = 0;
    } else {
        glEnable(GL_LIGHT1);
        lights_on[0] = 1;
    }
    break;
  case '2':
    if (lights_on[1]) {
        glDisable(GL_LIGHT2);
        lights_on[1] = 0;
    } else {
        glEnable(GL_LIGHT2);
        lights_on[1] = 1;
    }
    break;
  case '3':
    if (lights_on[2]) {
        glDisable(GL_LIGHT3);
        lights_on[2] = 0;
    } else {
        glEnable(GL_LIGHT3);
        lights_on[2] = 1;
    }
    break;
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
  camPosition.y = lookAtPoint.y + camDistance * sin(camAngle[Y]);
  camPosition.x = lookAtPoint.x + camDistance * cos(camAngle[Y]) * sin(camAngle[X]);
  camPosition.z = lookAtPoint.z + camDistance * cos(camAngle[Y]) * cos(camAngle[X]);
  glutPostRedisplay();
}



int main(int argc, char **argv) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      cpw[i][j][X] = cp[i][j][X] * w[i][j];
      cpw[i][j][Y] = cp[i][j][Y] * w[i][j];
      cpw[i][j][Z] = cp[i][j][Z] * w[i][j];
      cpw[i][j][3] = w[i][j];
    }
  }

  for(int i = 0; i < 7; i++) {
    for(int j = 0; j < 4; j++) {
        PNW[i][X] = PN[i][X] * 1;
        PNW[i][Y] = PN[i][Y] * 1;
        PNW[i][Z] = 0 * 1;
        PNW[i][3] = 1;
    }
  }

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowPosition(10, 10);
  glutInitWindowSize(800, 800);
  (void)glutCreateWindow("ESERCIZIO 2");
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
