#ifndef UTILS_H
#define UTILS_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
  double x;
  double y;
  double z;
} Point;

typedef struct {
  unsigned int numVerts;
  Point normal;
  Point *verts;
} Face;

typedef struct {
    unsigned int numFaces;
    Face *faces;
} Mesh;

enum {LEFT, RIGHT, BOTTOM, TOP, NEAR_PLANE, FAR_PLANE};

static const int second = 360.0/60.0;
static const int minute = 360.0/60.0;
static const int hour = 360.0/12.0;

static const double white[] = {1.0, 1.0, 1.0};
static const double black[] = {0.0, 0.0, 0.0};
static const double blue[] = {0.0, 0.0, 1.0};
static const double red[] = {1.0, 0.0, 0.0};
static const double green[] = {0.0, 1.0, 0.0};

void checkErrors(const char *label);
Point getPointOnCircumference(const Point *center, const double radius, const double angle);
int drawInscribedPolygon(const Point *center, const double radius, const int sides);
void drawRectangle(const Point points[4]);
void drawPrism(const int sides, const Point *lower_base_center, const Point *upper_base_center, const double radius);
void draw2DClock(const Point *center, const double radius, const double secondsRotation, const double minutesRotation, const double hoursRotation, const double bg[3], const double fg[3]);
void draw3DClock(const Point *center, const double radius, const double secondsRotation, const double minutesRotation, const double hoursRotation, const double bg[3], const double fg[3]);
void drawPolyedron(const Mesh *mesh);
Point newell(const Point *verts, const unsigned int numVerts);
void plotFunction(double (*f)(const double, const double, const double), const double domain[2][2], const double step, const double k);
Point casteljau(const Point *P, const unsigned int N, const double t);
void bezierCurve(const Point *P, const unsigned int N, const double a, const double b);
int toHomogeneousCoordinates(double pw[4], const Point *p, const double w);
void rationalBezierCurve(const Point *CP, const double *w, const unsigned int N);
int checkContinuity(const Point *CP1, const unsigned int N1, const Point *CP2, const unsigned int N2);
int compositeBezierCurve(const Point *CP1, const unsigned int N1, const double *w1, const Point *CP2, const unsigned int N2, const double *w2);

#endif // UTILS_H
