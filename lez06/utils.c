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

Point getPointOnCircumference(const Point *center, const double radius,
                              const double angle) {
  Point p;
  p.x = center->x + radius * cos(angle);
  p.y = center->y + radius * sin(angle);
  p.z = center->z;
  return p;
}

int drawInscribedPolygon(const Point *center, const double radius,
                         const int sides, const double rgb[3]) {
  double angle, x, y, z;
  int i;
  if (sides < 3) {
    return -1;
  }
  glColor3dv(rgb);
  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(center->x, center->y, center->z);

  for (i = 0; i <= sides; i++) {
    angle = 2 * M_PI * i / sides;
    Point p = getPointOnCircumference(center, radius, angle);
    glVertex3f(p.x, p.y, p.z);
  }
  glEnd();
  return GL_NO_ERROR;
}

void drawRectangle(const Point points[4], const double rgb[3]) {
  int i;
  glColor3dv(rgb);
  glBegin(GL_QUADS);
  for (i = 0; i < 4; i++) {
    glVertex3f(points[i].x, points[i].y, points[i].z);
  }
  glEnd();
}
