#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

/**
Poliedro - casa
*/

typedef struct {
  double x, y, z;
} Point3D;

typedef struct  {
  Point3D v1, v2, v3, v4;
} Quad;

typedef struct {
  Point3D v1, v2, v3, v4, v5;
} Pentagon;

typedef struct {
    const unsigned int numVerts;
    const unsigned int numFaces;
    Point3D *verts;
    Point3D *norms;
} Mesh;

Point3D eyePosition = {0.0, 0.0, -5.0};
Point3D lookAtPoint = {0.0, 0.0, 0.0};
Point3D upVector = {0.0, 1.0, 0.0};

#define L 0.5
const unsigned int numFaces = 6;
const unsigned int numVerts = 8;
Point3D verts[8] = {
    // Lato sinistro
    {-L, -L, -L},
    {-L, -L, L},
    {-L, L, -L},
    {-L, L, L}, 
    // Lato destro
    {L, -L, -L},
    {L, -L, L},
    {L, L, -L},
    {L, L, L}
};

// Bottom y=-L: 0, 1, 4, 5
// Top y=L: 2, 3, 6, 7
// Sx x=-L: 0, 1, 2, 3
// Dx x=L: 4, 5, 6, 7
// Front z=L: 1, 3, 5, 7
// Back z=-L: 0, 2, 4, 6

Point3D newell(Point3D *verts, unsigned int numVerts) {
    Point3D normal = {0.0, 0.0, 0.0};
    for (unsigned int i = 0; i < numVerts; i++) {
        Point3D p = verts[i];
        Point3D next = verts[(i + 1) % numVerts];
        normal.x += (p.y - next.y) * (p.z + next.z);
        normal.y += (p.z - next.z) * (p.x + next.x);
        normal.z += (p.x - next.x) * (p.y + next.y);
    }
    return normal;
}

Point3D computeNormal(Mesh *mesh) {
    Point3D normal = {0.0, 0.0, 0.0};
    for (unsigned int face = 0; face < mesh->numFaces; face++) {
        // 0: Sx, 1: Dx, 2: Front, 3: Back, 4: Bottom, 5: Top
        Point3D faceVerts[4];
        for (unsigned int j = 0; j < 4; j++) {
            faceVerts[j] = mesh->verts[(face * 4 + j) % mesh->numVerts];
    }
        Point3D faceNormal = newell(faceVerts, 4);
        normal.x += faceNormal.x;
        normal.y += faceNormal.y;
        normal.z += faceNormal.z;
    }
    return normal;
}


void reshape(int width, int height) {
  if (height == 0) height = 1; // Prevent division by zero
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (double)width / (double)height, 0.1, 100.0);

  glMatrixMode(GL_MODELVIEW);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  
  glPointSize(1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
//   gluLookAt(eyePosition.x, eyePosition.y, eyePosition.z,
//             lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
//             upVector.x, upVector.y, upVector.z);


  glPushMatrix();
  glBegin(GL_QUADS);
    glColor3d(1.0, 0.0, 0.0);
    
  glEnd();
  glPopMatrix();

  glFlush();
}

GLvoid keyboard(unsigned char key, int x, int y) {
  
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(500, 500);
  int window = glutCreateWindow("Poliedro - Casa");
//   glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}