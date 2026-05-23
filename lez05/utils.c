#include "utils.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

void checkErrors(const char *label) {
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    fprintf(stderr, "%s: %s\n", label, gluErrorString(err));
  }
}

void getPointOnCircumference(Point dest, const Point center, const double radius, const double angle) {
  dest[X] = center[X] + radius * cos(angle);
  dest[Y] = center[Y] + radius * sin(angle);
}

int drawStar(const Point center, const int sides, const double r1, const double r2) {
  const double radius[2] = {r1, r2};
  double angle;
  int i;
  if (sides < 3) {
    return -1;
  }
  glBegin(GL_TRIANGLE_FAN);
    glVertex2d(center[X], center[Y]);

    for (i = 0; i <= 2*sides; i++) {
      angle = 2 * M_PI * i / sides;
      Point p;
      getPointOnCircumference(p, center, radius[i % 2], angle);
      glVertex2d(p[X], p[Y]);
    }
  glEnd();
  return GL_NO_ERROR;
}
