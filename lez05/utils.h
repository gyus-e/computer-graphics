#ifndef UTILS_H
#define UTILS_H

#define X 0
#define Y 1
#define Z 2
typedef double Point[2];

void checkErrors(const char *label);
void getPointOnCircumference(Point dest, const Point center, const double radius, const double angle);
int drawStar(const Point center, const int sides, const double r1, const double r2);

#endif // UTILS_H
