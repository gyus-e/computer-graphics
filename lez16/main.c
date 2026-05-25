/*
Disegnare un pesce che nuota seguendo un percorso  sinusoidale.
Il punto di vista deve essere posizionato vicino al centro del tracciato del percorso e lo sguardo deve seguire il pesce.
• Caricare l’immagine fish.rgba
• Utilizzare glutIdleFunc per animare il supporto dell’immagine.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

enum {X, Y, Z};

const double step = 1.0;
const double fishDistance = 30.0;
const double upVector[3] = {0.0, 1.0, 0.0};
double camPosition[3] = {0.0, 0.0, 0.0};
double fishPosition[3] = {0.0, 0.0, fishDistance};

unsigned * read_texture(char *name, int *width, int *height, int *components);

void reshape(int width, int height) {
  if (height == 0)
    height = 1;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(20.0, (double)width / (double)height, 20.0, 100.0);
  glMatrixMode(GL_MODELVIEW);
}

void display() {
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
    gluLookAt(camPosition[X], camPosition[Y], camPosition[Z], fishPosition[X],
                fishPosition[Y], fishPosition[Z], upVector[X], upVector[Y], upVector[Z]);
    glPointSize(1.0);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();
      glTranslated(fishPosition[X], fishPosition[Y], fishPosition[Z]);
      glRotated(-90.0, 0.0, 1.0, 0.0);
      GLUquadricObj *fish = gluNewQuadric();
      gluQuadricDrawStyle(fish, GLU_FILL); 
      gluQuadricOrientation(fish, GLU_OUTSIDE);
      gluQuadricNormals(fish, GLU_FLAT);
      gluCylinder(fish, 0.5, 0.5, 1.5, 30, 10);
    glPopMatrix();

  glPopMatrix();
  glutSwapBuffers();
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowPosition(10, 10);
  glutInitWindowSize(800, 800);
  (void)glutCreateWindow("TEXTURE");
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}