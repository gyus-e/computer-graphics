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

void drawPolyedron(const Mesh *mesh){
  Face f;
  for (int i = 0; i < mesh->numFaces; i++) {
    f = mesh->faces[i];
    glBegin(GL_POLYGON);
      glNormal3d(f.normal[X], f.normal[Y], f.normal[Z]);
      for (int j = 0; j < f.numVerts; j++) {
        glVertex3d(f.verts[j][X], f.verts[j][Y], f.verts[j][Z]);
      }
    glEnd();
  }
}

void newell(Point dest, const Point verts[], const unsigned int numVerts) {
    Point normal = {0.0, 0.0, 0.0};
    for (unsigned int i = 0; i < numVerts; i++) {
        Point p, next;
        p[X] = verts[i][X];
        p[Y] = verts[i][Y];
        p[Z] = verts[i][Z];
        next[X] = verts[(i + 1) % numVerts][X];
        next[Y] = verts[(i + 1) % numVerts][Y];
        next[Z] = verts[(i + 1) % numVerts][Z];
        normal[X] += (p[Y] - next[Y]) * (p[Z] + next[Z]);
        normal[Y] += (p[Z] - next[Z]) * (p[X] + next[X]);
        normal[Z] += (p[X] - next[X]) * (p[Y] + next[Y]);
    }
    dest[X] = normal[X];
    dest[Y] = normal[Y];
    dest[Z] = normal[Z];
}

void copyPoint(Point dest, const Point src) {
  dest[X] = src[X];
  dest[Y] = src[Y];
  dest[Z] = src[Z];
}
