#ifndef UTILS_H
#define UTILS_H

typedef double Point[3];
enum {X, Y, Z};
enum {LEFT, RIGHT, BOTTOM, TOP, NEAR_PLANE, FAR_PLANE};

extern const float white[3];
extern const float black[3];
extern const float red[3];
extern const float green[3];
extern const float blue[3];

void checkErrors(const char *label);
void getPointOnCircumference(Point dest, const Point center, const double radius, const double angle);
int drawInscribedPolygon(const Point center, const double radius, const int sides);
void draw2DClock(const Point center, const double radius, const double secondsRotation, const double minutesRotation, const double hoursRotation, const float bg[3], const float fg[3]);
void draw3DClock(const Point center, const double radius, const double secondsRotation, const double minutesRotation, const double hoursRotation, const float bg[3], const float fg[3]);

#endif // UTILS_H
