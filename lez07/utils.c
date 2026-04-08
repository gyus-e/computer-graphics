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

int drawStar(const Point *center, const int sides, const double r1, const double r2, const double rgb[3]) {
  const double radius[2] = {r1, r2};
  double angle;
  int i;
  if (sides < 3) {
    return -1;
  }
  glColor3dv(rgb);
  glBegin(GL_TRIANGLE_FAN);
  glVertex3d(center->x, center->y, center->z);

  for (i = 0; i <= 2*sides; i++) {
    angle = 2 * M_PI * i / sides;
    Point p = getPointOnCircumference(center, radius[i % 2], angle);
    glVertex3d(p.x, p.y, p.z);
  }
  glEnd();
  return GL_NO_ERROR;
}

void drawPrism(const GLint sides,
              const Point *lower_base_center,
              const Point *upper_base_center,
              const GLdouble radius,
              const GLdouble rgb[3]) {
  Point points[4];
  GLdouble angle, x, y, z;
  int i;

  drawInscribedPolygon(lower_base_center, radius, sides, rgb);
  drawInscribedPolygon(upper_base_center, radius, sides, rgb);
  for (i = 0; i <= sides; i++) {
    angle = 2 * M_PI * i / sides;
    points[0] = getPointOnCircumference(lower_base_center, radius, angle);
    points[1] = getPointOnCircumference(upper_base_center, radius, angle);

    angle = 2 * M_PI * (i + 1) / sides;
    points[2] = getPointOnCircumference(upper_base_center, radius, angle);
    points[3] = getPointOnCircumference(lower_base_center, radius, angle);

    drawRectangle(points, rgb);
  }
}