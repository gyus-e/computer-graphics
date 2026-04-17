#include "utils.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

/**
Poliedro - casa
*/
#define I 0.5
#define O -0.5

const Point lookAtPoint = {0.0, 0.0, 0.0};
const Point upVector = {0.0, 1.0, 0.0};
Point eyePosition = {0.0, 0.0, 2.0};

const unsigned int numFaces = 6;
const unsigned int numVerts = 8;
const Point verts[8] = {
  {O, O, O},
  {O, O, I},
  {O, I, O},
  {O, I, I},
  {I, O, O},
  {I, O, I},  
  {I, I, O},
  {I, I, I}
};



void rotateEyePosition_x(double angle) {
  double s = sin(angle);
  double c = cos(angle);

  double x = eyePosition.x * c - eyePosition.z * s;
  double z = eyePosition.x * s + eyePosition.z * c;


  eyePosition.x = x;
  eyePosition.z = z;
}



void rotateEyePosition_y(double angle) {
  double s = sin(angle);
  double c = cos(angle);

  double y = eyePosition.y * c - eyePosition.z * s;
  double z = eyePosition.y * s + eyePosition.z * c;

  eyePosition.y = y;
  eyePosition.z = z;
}



void reshape(int width, int height) {
  if (height == 0) height = 1;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (double)width / (double)height, 0.1, 50.0);
  glMatrixMode(GL_MODELVIEW);
}



void display() {
  
  const Point front_verts[4] = {verts[1], verts[5], verts[7], verts[3]}; // vertici dispari, senso antiorario
  const Point back_verts[4] = {verts[0], verts[2], verts[6], verts[4]}; // vertici pari, senso orario
  const Point base_verts[4] = {verts[0], verts[4], verts[5], verts[1]};
  const Point top_verts[4] = {verts[2], verts[3], verts[7], verts[6]};
  const Point left_verts[4] = {verts[0], verts[1], verts[3], verts[2]};
  const Point right_verts[4] = {verts[4], verts[6], verts[7], verts[5]};

  const Point frontFaceNormal = newell(front_verts, 4);
  const Point backFaceNormal = newell(back_verts, 4);
  const Point baseNormal = newell(base_verts, 4);
  const Point topNormal = newell(top_verts, 4);
  const Point leftNormal = newell(left_verts, 4);
  const Point rightNormal = newell(right_verts, 4);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearDepth(50.0);
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
  glColor3d(0.0, 0.0, 1.0);
  glBegin(GL_QUADS);

    glNormal3d(backFaceNormal.x, backFaceNormal.y, backFaceNormal.z);
    for (int i = 0; i < 4; i++) {
      glVertex3d(back_verts[i].x, back_verts[i].y, back_verts[i].z);
    }

    glNormal3d(leftNormal.x, leftNormal.y, leftNormal.z);
    for (int i = 0; i < 4; i++) {
      glVertex3d(left_verts[i].x, left_verts[i].y, left_verts[i].z);
    }

    glNormal3d(baseNormal.x, baseNormal.y, baseNormal.z);
    for (int i = 0; i < 4; i++) {
      glVertex3d(base_verts[i].x, base_verts[i].y, base_verts[i].z);
    }

    glNormal3d(rightNormal.x, rightNormal.y, rightNormal.z);
    for (int i = 0; i < 4; i++) {
      glVertex3d(right_verts[i].x, right_verts[i].y, right_verts[i].z);
    }

    glNormal3d(topNormal.x, topNormal.y, topNormal.z);
    for (int i = 0; i < 4; i++) {
      glVertex3d(top_verts[i].x, top_verts[i].y, top_verts[i].z);
    }

    glNormal3d(frontFaceNormal.x, frontFaceNormal.y, frontFaceNormal.z);
    for (int i = 0; i < 4; i++) {
      glVertex3d(front_verts[i].x, front_verts[i].y, front_verts[i].z);
    }
  glEnd();
  glPopMatrix();

  glPopMatrix();
  glFlush();
}



GLvoid keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 'a':
      rotateEyePosition_x(-M_PI / 16.0);
      break;
    case 'd':
      rotateEyePosition_x(M_PI / 16.0);
      break;
    case 'w':
      rotateEyePosition_y(M_PI / 16.0);
      break;
    case 's':
      rotateEyePosition_y(-M_PI / 16.0);
      break;
    case 'r':
      eyePosition = (Point){0.0, 0.0, 2.0};
      break;
    case 27: // ESC
      exit(0);
      break;
  }
  glutPostRedisplay();
}



int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(500, 500);
  (void)glutCreateWindow("Poliedro - Casa");
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
