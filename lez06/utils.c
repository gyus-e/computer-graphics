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

void drawRectangle(const Point points[4]) {
  int i;
  glBegin(GL_QUADS);
  for (i = 0; i < 4; i++) {
    glVertex3f(points[i][X], points[i][Y], points[i][Z]);
  }
  glEnd();
}

void drawPrism(const GLint sides,
              const Point lower_base_center,
              const Point upper_base_center,
              const GLdouble radius) {
  Point points[4];
  GLdouble angle, x, y, z;
  int i;

  drawInscribedPolygon(lower_base_center, radius, sides);
  drawInscribedPolygon(upper_base_center, radius, sides);
  for (i = 0; i <= sides; i++) {
    angle = 2 * M_PI * i / sides;
    getPointOnCircumference(points[0], lower_base_center, radius, angle);
    getPointOnCircumference(points[1], upper_base_center, radius, angle);

    angle = 2 * M_PI * (i + 1) / sides;
    getPointOnCircumference(points[2], upper_base_center, radius, angle);
    getPointOnCircumference(points[3], lower_base_center, radius, angle);

    drawRectangle(points);
  }
}