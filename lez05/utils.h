#ifndef UTILS_H
#define UTILS_H

typedef struct {
  double x;
  double y;
} Point;

void checkErrors(const char *label);
Point getPointOnCircumference(const Point *center, const double radius, const double angle);
int drawStar(const Point *center, const int sides, const double r1, const double r2, const double rgb[3]);

#endif // UTILS_H
