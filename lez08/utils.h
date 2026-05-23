#ifndef UTILS_H
#define UTILS_H

typedef double Point[3];
enum {X, Y, Z};
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
void getPointOnCircumference(Point dest, const Point center, const double radius, const double angle);
int drawInscribedPolygon(const Point center, const double radius, const int sides);
void drawPolyedron(const Mesh *mesh);
void newell(Point dest, const Point verts[], const unsigned int numVerts);
void copyPoint(Point dest, const Point src);

#endif // UTILS_H
