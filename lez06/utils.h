#ifndef UTILS_H
#define UTILS_H

typedef struct {
  double x;
  double y;
  double z;
} Point;

enum {LEFT, RIGHT, BOTTOM, TOP, NEAR_PLANE, FAR_PLANE};

void checkErrors(const char *label);
Point getPointOnCircumference(const Point *center, const double radius, const double angle);
int drawInscribedPolygon(const Point *center, const double radius, const int sides, const double rgb[3]);
void drawRectangle(const Point points[4], const double rgb[3]);

#endif // UTILS_H
