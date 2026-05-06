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



void draw2DClock(const Point *center, const double radius, const double secondsRotation, const double minutesRotation, const double hoursRotation, const double bg[3], const double fg[3]) {
  const int sides = 360;

  drawInscribedPolygon(center, radius, sides, bg);
  
  glColor3dv(fg);
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
    glVertex3d(center->x, center->y, center->z);
    glVertex3d(0.0, 0.45, 0.0);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glRotated(minutesRotation, 0.0, 0.0, 1.0);
  glBegin(GL_LINES);
    glVertex3d(center->x, center->y, center->z);
    glVertex3d(0.0, 0.4, 0.0);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glRotated(hoursRotation, 0.0, 0.0, 1.0);
  glBegin(GL_LINES);
    glVertex3d(center->x, center->y, center->z);
    glVertex3d(0.25, 0.0, 0.0);
  glEnd();
  glPopMatrix();
}



void draw3DClock(const Point *center, const double radius, const double secondsRotation, const double minutesRotation, const double hoursRotation, const double bg[3], const double fg[3]) {
  Point backCenter = {center->x, center->y, center->z - 0.5};
  draw2DClock(center, radius, secondsRotation, minutesRotation, hoursRotation, bg, fg);
  draw2DClock(&backCenter, radius, secondsRotation, minutesRotation, hoursRotation, bg, fg);
}



void drawPolyedron(const Mesh *mesh){
  Face *f;
  for (int i = 0; i < mesh->numFaces; i++) {
    f = &(mesh->faces[i]);
    glBegin(GL_POLYGON);
      glNormal3d(f->normal.x, f->normal.y, f->normal.z);
      for (int j = 0; j < f->numVerts; j++) {
        glVertex3d(f->verts[j].x, f->verts[j].y, f->verts[j].z);
      }
    glEnd();
  }
}



Point newell(const Point *verts, const unsigned int numVerts) {
    Point normal = {0.0, 0.0, 0.0};
    for (unsigned int i = 0; i < numVerts; i++) {
        const Point p = verts[i];
        const Point next = verts[(i + 1) % numVerts];
        normal.x += (p.y - next.y) * (p.z + next.z);
        normal.y += (p.z - next.z) * (p.x + next.x);
        normal.z += (p.x - next.x) * (p.y + next.y);
    }
    return normal;
}



void plotFunction(double (*f)(const double, const double, const double), const double domain[2][2], const double step, const double k) {
  for (double x = domain[0][0]; x <= domain[0][1]; x += step) {
    for (double z = domain[1][0]; z <= domain[1][1]; z += step) {
        glBegin(GL_QUADS);
          Point p0 = {x, f(x, z, k), z};
          Point p1 = {x, f(x, z + step, k), z + step};
          Point p2 = {x + step, f(x + step, z + step, k), z + step};
          Point p3 = {x + step, f(x + step, z, k), z};
          Point normal = newell((Point[]){p0, p1, p2, p3}, 4);      
          glNormal3d(normal.x, normal.y, normal.z);
          glVertex3d(p0.x, p0.y, p0.z);
          glVertex3d(p1.x, p1.y, p1.z);
          glVertex3d(p2.x, p2.y, p2.z);
          glVertex3d(p3.x, p3.y, p3.z);
        glEnd();
      }
    }
}



Point casteljau(const Point *P, const unsigned int N, const double t) {
    Point *p = malloc(N * sizeof(Point));
    for (int i = 0; i < N; i++) {
        p[i].x = P[i].x;
        p[i].y = P[i].y;
        p[i].z = P[i].z;
    }
    for (int r = 1; r < N; r++) {
        for (int i = 0; i < N - r; i++) {
            double x = (1-t)*p[i].x + t*p[i+1].x;
            double y = (1-t)*p[i].y + t*p[i+1].y;
            double z = (1-t)*p[i].z + t*p[i+1].z;
            p[i].x = x;
            p[i].y = y;
            p[i].z = z;
        }
    }
    Point result = p[0];
    free(p);
    return result;
}



void bezierCurve(const Point *P, const unsigned int N) {
  glBegin(GL_LINE_STRIP);
    glVertex3d(P[0].x, P[0].y, P[0].z);
    for (double t = 0.0; t <= 1.0; t += 0.01) {
      Point p = casteljau(P, N, t);
      glVertex3d(p.x, p.y, p.z);
    }
  glEnd();
}
