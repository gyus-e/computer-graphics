#ifndef UTILS_H
#define UTILS_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef double Point[4];
enum {X, Y, Z, W};
enum {LEFT, RIGHT, BOTTOM, TOP, NEAR_PLANE, FAR_PLANE};

typedef struct {
  unsigned int numVerts;
  Point normal;
  Point *verts;
} Face;

typedef struct {
    unsigned int numFaces;
    Face *faces;
} Mesh;

extern const float white[3];
extern const float black[3];
extern const float red[3];
extern const float green[3];
extern const float blue[3];

void checkErrors(const char *label);
void copyPoint(Point dest, const Point src);
void toHomogeneousCoordinates(Point dest, const Point p, const double w);
int checkContinuity1(const Point *CP1, const unsigned int N1, const Point *CP2, const unsigned int N2);
int checkContinuity2(const Point *CP1, const unsigned int N1, const Point *CP2, const unsigned int N2);

void newell(Point dest, const Point verts[], const unsigned int numVerts);
void casteljau(Point dest, const Point *P, const unsigned int N, const double t);

void getPointOnCircumference(Point dest, const Point center, const double radius, const double angle);
int drawInscribedPolygon(const Point center, const double radius, const int sides);
void drawPolyedron(const Mesh *mesh);
void plotFunction(double (*f)(const double, const double, const double), const double domain[2][2], const double step, const double k);
void bezierCurve(const Point *P, const unsigned int N, const double a, const double b);
void rationalBezierCurve(const Point *CP, const double *w, const unsigned int N);
int compositeBezierCurve(const Point *CP1, const unsigned int N1, const double *w1, const Point *CP2, const unsigned int N2, const double *w2);

#endif // UTILS_H
