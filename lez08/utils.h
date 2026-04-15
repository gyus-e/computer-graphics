#ifndef UTILS_H
#define UTILS_H

typedef struct {
  double x;
  double y;
  double z;
} Point;

typedef struct {
    const unsigned int numVerts;
    const unsigned int numFaces;
    Point *verts;
    Point *norms;
} Mesh;

enum {LEFT, RIGHT, BOTTOM, TOP, NEAR_PLANE, FAR_PLANE};

static const int second = 360.0/60.0;
static const int minute = 360.0/60.0;
static const int hour = 360.0/12.0;

static const double white[] = {1.0, 1.0, 1.0};
static const double black[] = {0.0, 0.0, 0.0};
static const double blue[] = {0.0, 0.0, 1.0};
static const double red[] = {1.0, 0.0, 0.0};

void checkErrors(const char *label);
Point getPointOnCircumference(const Point *center, const double radius, const double angle);
int drawInscribedPolygon(const Point *center, const double radius, const int sides, const double rgb[3]);
void drawRectangle(const Point points[4], const double rgb[3]);
void drawPrism(const int sides, const Point *lower_base_center, const Point *upper_base_center, const double radius, const double rgb[3]);
void draw2DClock(const Point *center, const double radius, const double secondsRotation, const double minutesRotation, const double hoursRotation, const double bg[3], const double fg[3]);
void draw3DClock(const Point *center, const double radius, const double secondsRotation, const double minutesRotation, const double hoursRotation, const double bg[3], const double fg[3]);
Point newell(const Point *verts, const unsigned int numVerts);

#endif // UTILS_H
