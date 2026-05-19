#include "utils.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>



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
                         const int sides) {
  double angle, x, y, z;
  int i;
  if (sides < 3) {
    return -1;
  }
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



void drawRectangle(const Point points[4]) {
  int i;
  glBegin(GL_QUADS);
    for (i = 0; i < 4; i++) {
      glVertex3f(points[i].x, points[i].y, points[i].z);
    }
  glEnd();
}



int drawStar(const Point *center, const int sides, const double r1, const double r2) {
  const double radius[2] = {r1, r2};
  double angle;
  int i;
  if (sides < 3) {
    return -1;
  }
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
              const GLdouble radius) {
  Point points[4];
  GLdouble angle, x, y, z;
  int i;

  drawInscribedPolygon(lower_base_center, radius, sides);
  drawInscribedPolygon(upper_base_center, radius, sides);
  for (i = 0; i <= sides; i++) {
    angle = 2 * M_PI * i / sides;
    points[0] = getPointOnCircumference(lower_base_center, radius, angle);
    points[1] = getPointOnCircumference(upper_base_center, radius, angle);

    angle = 2 * M_PI * (i + 1) / sides;
    points[2] = getPointOnCircumference(upper_base_center, radius, angle);
    points[3] = getPointOnCircumference(lower_base_center, radius, angle);

    drawRectangle(points);
  }
}



void draw2DClock(const Point *center, const double radius, const double secondsRotation, const double minutesRotation, const double hoursRotation, const double bg[3], const double fg[3]) {
  const int sides = 360;

  glColor3dv(bg);
  drawInscribedPolygon(center, radius, sides);
  
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



Point casteljau(const Point *CP, const unsigned int N, const double t) {
    Point *p = malloc(N * sizeof(Point));
    for (int i = 0; i < N; i++) {
        p[i].x = CP[i].x;
        p[i].y = CP[i].y;
        p[i].z = CP[i].z;
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



void bezierCurve(const Point *CP, const unsigned int N, const double a, const double b) {
  glBegin(GL_LINE_STRIP);
    glVertex3d(CP[0].x, CP[0].y, CP[0].z);
    for (double s = a; s <= b; s += 0.01) {
      const double t = (s - a) / (b - a);
      Point p = casteljau(CP, N, t);
      glVertex3d(p.x, p.y, p.z);
    }
  glEnd();
}



int omogenize3dp(double pw[4], const Point *p, const double w) {
  if (pw == NULL || p == NULL) {
    fprintf(stderr, "Error: pw and p cannot be NULL.\n");
    return 1;
  }
  pw[0] = p->x * w;
  pw[1] = p->y * w;
  pw[2] = p->z * w;
  pw[3] = w;
  return 0;
}



int omogenize3dv(double pw[4], const double p[3], const double w) {
  if (pw == NULL || p == NULL) {
    fprintf(stderr, "Error: pw and p cannot be NULL.\n");
    return 1;
  }
  pw[X] = p[X] * w;
  pw[Y] = p[Y] * w;
  pw[Z] = p[Z] * w;
  pw[3] = w;
  return 0;
}



void rationalBezierCurve(const Point CP[], const double w[], const unsigned int N) {
  double *cpw = malloc(N * 4 * sizeof(double));

  for (int i = 0; i < N; i++) {
    (void)omogenize3dp(&cpw[i*4], &CP[i], w[i]);
  }

  glMap1d(GL_MAP1_VERTEX_4, 0.0, 1.0, 4, N, cpw);
  glEnable(GL_MAP1_VERTEX_4);
  
  glMapGrid1d(30, 0, 1);
  glEvalMesh1(GL_LINE, 0, 30);

  free(cpw);
}



int checkContinuity(const Point *CP1, const unsigned int N1,
                    const Point *CP2, const unsigned int N2) {
  if (CP1[N1-1].x != CP2[0].x || CP1[N1-1].y != CP2[0].y || CP1[N1-1].z != CP2[0].z) {
    fprintf(stderr, "Error: The last control point of the first curve must be the same as the first control point of the second curve.\n");
    return 0;
  }
  if (CP1[N1-1].x - CP1[N1-2].x != CP2[1].x - CP2[0].x || CP1[N1-1].y - CP1[N1-2].y != CP2[1].y - CP2[0].y || CP1[N1-1].z - CP1[N1-2].z != CP2[1].z - CP2[0].z) {
    fprintf(stderr, "Warning: The tangent at the joining point is not the same for both curves.\n");
    return 2;
  }
  return 1;
}



int compositeBezierCurve(const Point *CP1, const unsigned int N1, const double *w1,
                          const Point *CP2, const unsigned int N2, const double *w2) {
  if (!checkContinuity(CP1, N1, CP2, N2)) {
    return 1;
  }
  rationalBezierCurve(CP1, w1, N1);
  rationalBezierCurve(CP2, w2, N2);
  return 0;
}



void nurbsError(GLenum errorCode) {
  const GLubyte *errorString = gluErrorString(errorCode);
  fprintf(stderr, "Errore NURBS: %s\n", errorString);
  exit(EXIT_FAILURE);
}
