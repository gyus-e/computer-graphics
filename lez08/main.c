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

Mesh house;
const unsigned int numFaces = 7;
const unsigned int numVerts = 10;
const Point verts[10] = {
  {O, O, O},
  {O, O, I},
  {O, I, O},
  {O, I, I},
  {I, O, O},
  {I, O, I},
  {I, I, O},
  {I, I, I},
  {0.0, 1.0, O},
  {0.0, 1.0, I}
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
  Face *f;
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

      glBegin(GL_POLYGON);
        f = &(house.faces[0]);
        glNormal3d(f->normal.x, f->normal.y, f->normal.z);
        for (int j = 0; j < f->numVerts; j++) {
          glVertex3d(f->verts[j].x, f->verts[j].y, f->verts[j].z);
        }
      glEnd();
      glBegin(GL_QUADS);
        for (int i = 1; i < house.numFaces - 1; i++) {
          f = &(house.faces[i]);
          
          glNormal3d(f->normal.x, f->normal.y, f->normal.z);
          for (int j = 0; j < f->numVerts; j++) {
            glVertex3d(f->verts[j].x, f->verts[j].y, f->verts[j].z);
          }
        }
      glEnd();
      glBegin(GL_POLYGON);
        f = &(house.faces[house.numFaces - 1]);
        glNormal3d(f->normal.x, f->normal.y, f->normal.z);
        for (int j = 0; j < f->numVerts; j++) {
          glVertex3d(f->verts[j].x, f->verts[j].y, f->verts[j].z);
        }
      glEnd();
    glPopMatrix();
  glPopMatrix();
  glFlush();
}



int main(int argc, char **argv) {
  house.numFaces = numFaces;
  house.faces = malloc(house.numFaces * sizeof(Face));

  house.faces[0].numVerts = 5;
  for (int i = 1; i < house.numFaces - 1; i++) {
    house.faces[i].numVerts = 4;
  }
  house.faces[house.numFaces - 1].numVerts = 5;

  for (int i = 0; i < house.numFaces; i++) {
    house.faces[i].verts = malloc(house.faces[i].numVerts * sizeof(Point));
  }

  // back
  house.faces[0].verts[0] = verts[0];
  house.faces[0].verts[1] = verts[2];
  house.faces[0].verts[2] = verts[8];
  house.faces[0].verts[3] = verts[6];
  house.faces[0].verts[4] = verts[4];

  // base 
  house.faces[1].verts[0] = verts[0];
  house.faces[1].verts[1] = verts[4];
  house.faces[1].verts[2] = verts[5];
  house.faces[1].verts[3] = verts[1];

  // left
  house.faces[2].verts[0] = verts[0];
  house.faces[2].verts[1] = verts[1];
  house.faces[2].verts[2] = verts[3];
  house.faces[2].verts[3] = verts[2];

  // right
  house.faces[3].verts[0] = verts[4];
  house.faces[3].verts[1] = verts[6];
  house.faces[3].verts[2] = verts[7];
  house.faces[3].verts[3] = verts[5];

  // roof left
  house.faces[4].verts[0] = verts[2];
  house.faces[4].verts[1] = verts[3];
  house.faces[4].verts[2] = verts[9];
  house.faces[4].verts[3] = verts[8];

  // roof right
  house.faces[5].verts[0] = verts[7];
  house.faces[5].verts[1] = verts[6];
  house.faces[5].verts[2] = verts[8];
  house.faces[5].verts[3] = verts[9];

  // front
  house.faces[6].verts[0] = verts[5];
  house.faces[6].verts[1] = verts[7];
  house.faces[6].verts[2] = verts[9];
  house.faces[6].verts[3] = verts[3];
  house.faces[6].verts[4] = verts[1];

  for (int i = 0; i < house.numFaces; i++) {
    house.faces[i].normal = newell(house.faces[i].verts, house.faces[i].numVerts);
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

  for (int i = 0; i < house.numFaces; i++) {
    free(house.faces[i].verts);
  }
  free(house.faces);
  return 0;
}
