#include "utils.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

/**
Poliedro - casa
*/
#define I 0.5
#define O -0.5

Point eyePosition = {0.0, 0.0, 2.0};
const Point lookAtPoint = {0.0, 0.0, 0.0};
const Point upVector = {0.0, 1.0, 0.0};

Mesh cube;
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
    for (int i = 0; i < cube.numFaces; i++) {
      Face *f = &(cube.faces[i]);
      
      glNormal3d(f->normal.x, f->normal.y, f->normal.z);
      for (int j = 0; j < f->numVerts; j++) {
        glVertex3d(f->verts[j].x, f->verts[j].y, f->verts[j].z);
      }
    }
  glEnd();
  glPopMatrix();

  glPopMatrix();
  glFlush();
}



int main(int argc, char **argv) {
  cube.numFaces = numFaces;
  cube.faces = malloc(numFaces * sizeof(Face));

  for (int i = 0; i < numFaces; i++) {
    cube.faces[i].numVerts = 4;
    cube.faces[i].verts = malloc(cube.faces[i].numVerts * sizeof(Point));
  }

  // back
  cube.faces[0].verts[0] = verts[0];
  cube.faces[0].verts[1] = verts[2];
  cube.faces[0].verts[2] = verts[6];
  cube.faces[0].verts[3] = verts[4];

  // left
  cube.faces[1].verts[0] = verts[0];
  cube.faces[1].verts[1] = verts[1];
  cube.faces[1].verts[2] = verts[3];
  cube.faces[1].verts[3] = verts[2];

  // base 
  cube.faces[2].verts[0] = verts[0];
  cube.faces[2].verts[1] = verts[4];
  cube.faces[2].verts[2] = verts[5];
  cube.faces[2].verts[3] = verts[1];

  // right
  cube.faces[3].verts[0] = verts[4];
  cube.faces[3].verts[1] = verts[6];
  cube.faces[3].verts[2] = verts[7];
  cube.faces[3].verts[3] = verts[5];

  // top
  cube.faces[4].verts[0] = verts[2];
  cube.faces[4].verts[1] = verts[3];
  cube.faces[4].verts[2] = verts[7];
  cube.faces[4].verts[3] = verts[6];

  // front
  cube.faces[5].verts[0] = verts[1];
  cube.faces[5].verts[1] = verts[5];
  cube.faces[5].verts[2] = verts[7];
  cube.faces[5].verts[3] = verts[3];

  for (int i = 0; i < cube.numFaces; i++) {
    cube.faces[i].normal = newell(cube.faces[i].verts, cube.faces[i].numVerts);
  }
  
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(500, 500);
  (void)glutCreateWindow("Poliedro - Casa");
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMainLoop();

  for (int i = 0; i < cube.numFaces; i++) {
    free(cube.faces[i].verts);
  }
  free(cube.faces);
  return 0;
}
