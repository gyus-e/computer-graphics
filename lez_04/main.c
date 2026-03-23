#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <GL/freeglut_std.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include "checkerror.h"


const char *names[] = {"ESERCIZIO 1", "ESERCIZIO 2", "ESERCIZIO 3", "ESERCIZIO 4"};
const float center[] = {0.0, 0.0};
const float r = 0.5;

int sides = 2;


//ESERCIZIO 1: cerchio blu con centro rosso (sfumato)
void es1(void) {
  float angle, x, y;
  int i;

  glPointSize(1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0, 0.0, 0.0); 
    glVertex2f(center[0], center[1]);

    glColor3f(0.0, 0.0, 1.0);
    for (i = 0; i <= 360; i++) {
      angle = M_PI * i / 180.0;
      x = center[0] + r * cos(angle);
      y = center[1] + r * sin(angle);
      glVertex2f(x, y);
    }
  glEnd();

  checkError(names[0]);
  glFlush();
}


//ESERCIZIO 2: poligono inscritto, numero lati cambia premendo spazio
void es2(void) {
  float angle, x, y;
  int i;

  glPointSize(1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0, 0.0, 0.0); 
    glVertex2f(center[0], center[1]);

    glColor3f(0.0, 0.0, 1.0);
    for (i = 0; i <= 360; i+=360/sides) {
      angle = M_PI * i / 180.0;
      x = center[0] + r * cos(angle);
      y = center[1] + r * sin(angle);
      glVertex2f(x, y);
    }
  glEnd();

  checkError(names[1]);
  glFlush();
}


//ESERCIZIO 3: ?
void es3(void) {
  glPointSize(1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glBegin(GL_TRIANGLE_FAN);
    
  glEnd();

  checkError(names[2]);
  glFlush();
}


// ESERCIZIO 4: cerchio blu con centro rosso (sfumato) + cerchio nero più piccolo al centro
void es4(void) {
  float angle, x, y;
  int i;

  glPointSize(1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0, 0.0, 0.0); 
    glVertex2f(center[0], center[1]);
    
    glColor3f(0.0, 0.0, 1.0);
    for (i = 0; i <= 360; i++) {
      angle = M_PI * i / 180.0;
      x = center[0] + r * cos(angle);
      y = center[1] + r * sin(angle);
      glVertex2f(x, y);
    }
  glEnd();

  glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.0, 0.0, 0.0); 
    glVertex2f(center[0], center[1]);
    for (int i = 0; i <= 360; i++) {
      float angle = i * M_PI / 180.0;
      float x = center[0] + r*0.5 * cos(angle);
      float y = center[1] + r*0.5 * sin(angle);
      glVertex2f(x, y);
    }
  glEnd();
  
  checkError(names[3]);
  glFlush();
}


void keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case ' ': // SPACE key
      sides = rand() % 13 + 4; // Random number of sides between 4 and 16
      glClearColor((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 1.0);
      glutPostRedisplay();
      break;
    case 27: // ESC key
      exit(0);
  }
}


void special(){

}


void mouse(){

}


int main(int argc, char **argv) {
  int size = 450;
  int padding = 100;
  int window[4];
  srand(time(NULL));
  

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB);


  glutInitWindowPosition(0, 0);
  glutInitWindowSize(size, size);
  window[0] = glutCreateWindow(names[0]);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glutDisplayFunc(es1);
  glutKeyboardFunc(keyboard);


  glutInitWindowPosition(size + padding, 0);
  glutInitWindowSize(size, size);
  window[1] = glutCreateWindow(names[1]);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glutDisplayFunc(es2);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutMouseFunc(mouse);


  glutInitWindowPosition(0, size + padding);
  glutInitWindowSize(size, size);
  window[2] = glutCreateWindow(names[2]);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glutDisplayFunc(es3);
  glutKeyboardFunc(keyboard);


  glutInitWindowPosition(size + padding, size + padding);
  glutInitWindowSize(size, size);
  window[3] = glutCreateWindow(names[3]);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glutDisplayFunc(es4);
  glutKeyboardFunc(keyboard);
  

  glutMainLoop();
  return 0;
}
