#include "utils.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

const float white[3] = {1.0, 1.0, 1.0};
const float black[3] = {0.0, 0.0, 0.0};
const float red[3] = {1.0, 0.0, 0.0};
const float green[3] = {0.0, 1.0, 0.0};
const float blue[3] = {0.0, 0.0, 1.0};

void checkErrors(const char *label) {
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    fprintf(stderr, "%s: %s\n", label, gluErrorString(err));
  }
}

void getPointOnCircumference(Point dest, const Point center, const double radius, const double angle) {
  dest[X] = center[X] + radius * cos(angle);
  dest[Y] = center[Y] + radius * sin(angle);
  dest[Z] = center[Z];
}

int drawInscribedPolygon(const Point center, const double radius,
                         const int sides) {
  double angle, x, y, z;
  int i;
  if (sides < 3) {
    return -1;
  }
  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(center[X], center[Y], center[Z]);

  for (i = 0; i <= sides; i++) {
    angle = 2 * M_PI * i / sides;
    Point p;
    getPointOnCircumference(p, center, radius, angle);
    glVertex3f(p[X], p[Y], p[Z]);
  }
  glEnd();
  return GL_NO_ERROR;
}

void draw2DClock(const Point center, const double radius, const double secondsRotation, const double minutesRotation, const double hoursRotation, const float bg[3], const float fg[3]) {
  const int second = 360.0/60.0;
  const int minute = 360.0/60.0;
  const int hour = 360.0/12.0;
  const int sides = 360;

  glColor3fv(bg);
  drawInscribedPolygon(center, radius, sides);
  
  glColor3fv(fg);
  for (int i = 12; i > 0 ; --i) {
    double angle = 2*M_PI - i*hour*2*M_PI/360.0 + M_PI/2.0;
    double x = (radius-0.1)*cos(angle);
    double y = (radius-0.1)*sin(angle);
    glRasterPos3d(x, y, 0.0);
    
    char number[3];
    sprintf(number, "%d", i);
    for (char *c = number; *c != '\0'; ++c) {
      glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
    }
  }
  
  glPushMatrix();
    glRotated(secondsRotation, 0.0, 0.0, 1.0);
    glBegin(GL_LINES);
      glVertex3d(center[X], center[Y], center[Z]);
      glVertex3d(0.0, 0.45, 0.0);
    glEnd();
  glPopMatrix();

  glPushMatrix();
    glRotated(minutesRotation, 0.0, 0.0, 1.0);
    glBegin(GL_LINES);
      glVertex3d(center[X], center[Y], center[Z]);
      glVertex3d(0.0, 0.4, 0.0);
    glEnd();
  glPopMatrix();

  glPushMatrix();
    glRotated(hoursRotation, 0.0, 0.0, 1.0);
    glBegin(GL_LINES);
      glVertex3d(center[X], center[Y], center[Z]);
      glVertex3d(0.25, 0.0, 0.0);
    glEnd();
  glPopMatrix();
}

void draw3DClock(const Point center, const double radius, const double secondsRotation, const double minutesRotation, const double hoursRotation, const float bg[3], const float fg[3]) {
  Point backCenter = {center[X], center[Y], center[Z] - 0.5};
  glColor3fv(bg);
  for (double depth = 0.0; depth <= 0.5; depth += 0.01) {
    backCenter[Z] = center[Z] - depth;
    drawInscribedPolygon(backCenter, radius, 360);
  }
  draw2DClock(center, radius, secondsRotation, minutesRotation, hoursRotation, bg, fg);
}