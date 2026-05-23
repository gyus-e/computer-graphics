#ifndef UTILS_H
#define UTILS_H

#define X 0
#define Y 1
#define Z 2
typedef double Point[3];
enum {LEFT, RIGHT, BOTTOM, TOP, NEAR_PLANE, FAR_PLANE};

extern const float white[3];
extern const float black[3];
extern const float red[3];
extern const float green[3];
extern const float blue[3];

void checkErrors(const char *label);
void getPointOnCircumference(Point dest, const Point center, const double radius, const double angle);
int drawInscribedPolygon(const Point center, const double radius, const int sides);
void drawRectangle(const Point points[4]);
void drawPrism(const int sides, const Point lower_base_center, const Point upper_base_center, const double radius);

#endif // UTILS_H
