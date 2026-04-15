#include "utils.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>

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


  glPushMatrix();
  glColor3d(1.0, 0.0, 0.0);
  glBegin(GL_QUADS);
    Point frontFaceNormal = newell(verts, 4);
    glNormal3d(frontFaceNormal.x, frontFaceNormal.y, frontFaceNormal.z);
    for (int i = 0; i < 4; i++) {
      glVertex3d(verts[i].x, verts[i].y, verts[i].z);
    }
    
    Point backFaceNormal = newell(verts + 4, 4);
    glNormal3d(backFaceNormal.x, backFaceNormal.y, backFaceNormal.z);
    for (int i = 4; i < 8; i++) {
      glVertex3d(verts[i].x, verts[i].y, verts[i].z);
    }
  glEnd();
  glPopMatrix();

  glFlush();
}



GLvoid keyboard(unsigned char key, int x, int y) {
  
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
