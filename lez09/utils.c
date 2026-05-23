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

void copyPoint(Point dest, const Point src) {
  dest[X] = src[X];
  dest[Y] = src[Y];
  dest[Z] = src[Z];
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
        copyPoint(p, verts[i]);
        copyPoint(next, verts[(i + 1) % numVerts]);
        normal[X] += (p[Y] - next[Y]) * (p[Z] + next[Z]);
        normal[Y] += (p[Z] - next[Z]) * (p[X] + next[X]);
        normal[Z] += (p[X] - next[X]) * (p[Y] + next[Y]);
    }
    dest[X] = normal[X];
    dest[Y] = normal[Y];
    dest[Z] = normal[Z];
}

void plotFunction(double (*f)(const double, const double, const double), const double domain[2][2], const double step, const double k) {
  for (double x = domain[0][0]; x <= domain[0][1]; x += step) {
    for (double z = domain[1][0]; z <= domain[1][1]; z += step) {
        Point p[4];
        p[0][X] = x;
        p[0][Y] = f(x, z, k);
        p[0][Z] = z;

        p[1][X] = x;
        p[1][Y] = f(x, z + step, k);
        p[1][Z] = z + step;
        
        p[2][X] = x + step;
        p[2][Y] = f(x + step, z + step, k);
        p[2][Z] = z + step;
        
        p[3][X] = x + step;
        p[3][Y] = f(x + step, z, k);
        p[3][Z] = z;
        
        Point normal;
        newell(normal, p, 4);
        
        glBegin(GL_QUADS);
          glNormal3d(normal[X], normal[Y], normal[Z]);
          glVertex3d(p[0][X], p[0][Y], p[0][Z]);
          glVertex3d(p[1][X], p[1][Y], p[1][Z]);
          glVertex3d(p[2][X], p[2][Y], p[2][Z]);
          glVertex3d(p[3][X], p[3][Y], p[3][Z]);
        glEnd();
      }
    }
}
