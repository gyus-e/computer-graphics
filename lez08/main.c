#include "utils.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

/**
Poliedro - casa
*/
#define L 0.5

Point eyePosition = {0.0, 0.0, -2.0};
Point lookAtPoint = {0.0, 0.0, 0.0};
Point upVector = {0.0, 1.0, 0.0};

const unsigned int numFaces = 6;
const unsigned int numVerts = 8;
Point verts[8] = {
  // Front face
    {-L, -L, L},
    {-L, L, L}, 
    {L, L, L},
    {L, -L, L},

  // Back face
    {-L, -L, -L},
    {-L, L, -L},
    {L, L, -L},
    {L, -L, -L}
};



void rotateEyePosition(double angle) {
  double s = sin(angle);
  double c = cos(angle);

  double x = eyePosition.x * c - eyePosition.z * s;
  double z = eyePosition.x * s + eyePosition.z * c;

  eyePosition.x = x;
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
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  
  glPointSize(1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glShadeModel(GL_FLAT);

  gluLookAt(eyePosition.x, eyePosition.y, eyePosition.z,
            lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
            upVector.x, upVector.y, upVector.z);

  // base_verts = v in verts where v.y == -L  
  Point base_verts[4] = {verts[0], verts[3], verts[4], verts[7]};

  Point baseNormal = newell(base_verts, 4);
  Point frontFaceNormal = newell(verts, 4);
  Point backFaceNormal = newell(verts + 4, 4);

  glPushMatrix();
  glColor3d(1.0, 0.0, 0.0);
  glBegin(GL_QUADS);

    glNormal3d(baseNormal.x, baseNormal.y, baseNormal.z);
    for (int i = 0; i < 4; i++) {
      glVertex3d(base_verts[i].x, base_verts[i].y, base_verts[i].z);
    }

    glNormal3d(frontFaceNormal.x, frontFaceNormal.y, frontFaceNormal.z);
    for (int i = 0; i < 4; i++) {
      glVertex3d(verts[i].x, verts[i].y, verts[i].z);
    }

    glNormal3d(backFaceNormal.x, backFaceNormal.y, backFaceNormal.z);
    for (int i = 4; i < 8; i++) {
      glVertex3d(verts[i].x, verts[i].y, verts[i].z);
    }

  glEnd();
  glPopMatrix();

  glFlush();
}



GLvoid keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 'a':
      rotateEyePosition(-M_PI / 16.0);
      break;
    case 'd':
      rotateEyePosition(M_PI / 16.0);
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
