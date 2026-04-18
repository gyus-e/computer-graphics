#include "utils.h"
#include "cJSON.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
Poliedro - casa
*/
#define I 0.5
#define O -0.5

Point eyePosition = {0.0, 0.0, 2.0};
const Point lookAtPoint = {0.0, 0.0, 0.0};
const Point upVector = {0.0, 1.0, 0.0};

Mesh mesh;
const unsigned int numFaces = 7;
const unsigned int numVerts = 10;
const Point verts[10] = {
  {O, O, O},
  {O, O, I},
  {O, I, O},
  {O, I, I},
  {I, O, O},
  {I, O, I},
  {I, I, O},
  {I, I, I},
  {0.0, 1.0, O},
  {0.0, 1.0, I}
};



void rotateEyePosition_x(double angle) {
  double s = sin(angle);
  double c = cos(angle);

  double x = eyePosition.x * c - eyePosition.z * s;
  double z = eyePosition.x * s + eyePosition.z * c;

  eyePosition.x = x;
  eyePosition.z = z;
}



void rotateEyePosition_y(double angle) {
  double s = sin(angle);
  double c = cos(angle);

  double y = eyePosition.y * c - eyePosition.z * s;
  double z = eyePosition.y * s + eyePosition.z * c;

  eyePosition.y = y;
  eyePosition.z = z;
}



GLvoid keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 'a':
      rotateEyePosition_x(-M_PI / 16.0);
      break;
    case 'd':
      rotateEyePosition_x(M_PI / 16.0);
      break;
    case 'w':
      rotateEyePosition_y(M_PI / 16.0);
      break;
    case 's':
      rotateEyePosition_y(-M_PI / 16.0);
      break;
    case 'r':
      eyePosition = (Point){0.0, 0.0, 2.0};
      break;
  }
  glutPostRedisplay();
}



void reshape(int width, int height) {
  if (height == 0) height = 1;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (double)width / (double)height, 0.1, 50.0);
  glMatrixMode(GL_MODELVIEW);
}



void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearDepth(50.0);
  glEnable(GL_DEPTH_TEST);
  
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  
  glPointSize(1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glShadeModel(GL_FLAT);

  glPushMatrix();
    gluLookAt(eyePosition.x, eyePosition.y, eyePosition.z,
              lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
              upVector.x, upVector.y, upVector.z);

    glPushMatrix();
      glColor3d(0.0, 0.0, 1.0);
      drawPolyedron(&mesh);
    glPopMatrix();
  glPopMatrix();
  glFlush();
}



void loadHouseMesh() {
  mesh.numFaces = numFaces;
  mesh.faces = malloc(mesh.numFaces * sizeof(Face));

  mesh.faces[0].numVerts = 5;
  for (int i = 1; i < mesh.numFaces - 1; i++) {
    mesh.faces[i].numVerts = 4;
  }
  mesh.faces[mesh.numFaces - 1].numVerts = 5;

  for (int i = 0; i < mesh.numFaces; i++) {
    mesh.faces[i].verts = malloc(mesh.faces[i].numVerts * sizeof(Point));
  }

  // back
  mesh.faces[0].verts[0] = verts[0];
  mesh.faces[0].verts[1] = verts[2];
  mesh.faces[0].verts[2] = verts[8];
  mesh.faces[0].verts[3] = verts[6];
  mesh.faces[0].verts[4] = verts[4];

  // base 
  mesh.faces[1].verts[0] = verts[0];
  mesh.faces[1].verts[1] = verts[4];
  mesh.faces[1].verts[2] = verts[5];
  mesh.faces[1].verts[3] = verts[1];

  // left
  mesh.faces[2].verts[0] = verts[0];
  mesh.faces[2].verts[1] = verts[1];
  mesh.faces[2].verts[2] = verts[3];
  mesh.faces[2].verts[3] = verts[2];

  // right
  mesh.faces[3].verts[0] = verts[4];
  mesh.faces[3].verts[1] = verts[6];
  mesh.faces[3].verts[2] = verts[7];
  mesh.faces[3].verts[3] = verts[5];

  // roof left
  mesh.faces[4].verts[0] = verts[2];
  mesh.faces[4].verts[1] = verts[3];
  mesh.faces[4].verts[2] = verts[9];
  mesh.faces[4].verts[3] = verts[8];

  // roof right
  mesh.faces[5].verts[0] = verts[7];
  mesh.faces[5].verts[1] = verts[6];
  mesh.faces[5].verts[2] = verts[8];
  mesh.faces[5].verts[3] = verts[9];

  // front
  mesh.faces[6].verts[0] = verts[5];
  mesh.faces[6].verts[1] = verts[7];
  mesh.faces[6].verts[2] = verts[9];
  mesh.faces[6].verts[3] = verts[3];
  mesh.faces[6].verts[4] = verts[1];

  for (int i = 0; i < mesh.numFaces; i++) {
    mesh.faces[i].normal = newell(mesh.faces[i].verts, mesh.faces[i].numVerts);
  }
}



void loadMeshFromFile() {
  FILE *fp = fopen("mesh.json", "r");
  if (fp == NULL) {
    fprintf(stderr, "Error opening file\n");
    exit(1);
  }
  char buffer[5192];
  int len = fread(buffer, 1, sizeof(buffer), fp);
  fclose(fp);

  cJSON *json = cJSON_ParseWithLength(buffer, len);
  if (json == NULL) {
      const char *error_ptr = cJSON_GetErrorPtr();
      if (error_ptr != NULL) {
          printf("Error: json is null. %s\n", error_ptr);
      }
      cJSON_Delete(json);
      exit(1);;
  }

  cJSON *faces_json = cJSON_GetObjectItemCaseSensitive(json, "faces");
  if (!cJSON_IsArray(faces_json)) {
      fprintf(stderr, "Error: faces is not an array\n");
      cJSON_Delete(json);
      exit(1);;
  }
  mesh.numFaces = cJSON_GetArraySize(faces_json);
  mesh.faces = malloc(mesh.numFaces * sizeof(Face));

  for (int i = 0; i < mesh.numFaces; i++) {
    cJSON *face_json = cJSON_GetArrayItem(faces_json, i);
    if (!cJSON_IsObject(face_json)) {
      fprintf(stderr, "Error: face %d is not an object\n", i);
      cJSON_Delete(json);
      exit(1);;
    }

    cJSON *verts_json = cJSON_GetObjectItemCaseSensitive(face_json, "verts");
    if (!cJSON_IsArray(verts_json)) {
      fprintf(stderr, "Error: verts for face %d is not an array\n", i);
      cJSON_Delete(json);
      exit(1);;
    }
    mesh.faces[i].numVerts = cJSON_GetArraySize(verts_json);
    mesh.faces[i].verts = malloc(mesh.faces[i].numVerts * sizeof(Point));

    for (int j = 0; j < mesh.faces[i].numVerts; j++) {
      cJSON *point_json = cJSON_GetArrayItem(verts_json, j);
      if (!cJSON_IsArray(point_json) || cJSON_GetArraySize(point_json) != 3) {
        fprintf(stderr, "Error: point %d for face %d is not an valid\n", j, i);
        cJSON_Delete(json);
        exit(1);;
      }

      cJSON *x_json = cJSON_GetArrayItem(point_json, 0);
      cJSON *y_json = cJSON_GetArrayItem(point_json, 1);
      cJSON *z_json = cJSON_GetArrayItem(point_json, 2);
      if (!cJSON_IsNumber(x_json) || !cJSON_IsNumber(y_json) || !cJSON_IsNumber(z_json)) {
        fprintf(stderr, "Error: vert %d for face %d has invalid coordinates\n", j, i);
        cJSON_Delete(json);
        exit(1);;
      }
      mesh.faces[i].verts[j] = (Point){x_json->valuedouble, y_json->valuedouble, z_json->valuedouble};
    }
  }

  cJSON_Delete(json);
  
  for (int i = 0; i < mesh.numFaces; i++) {
    mesh.faces[i].normal = newell(mesh.faces[i].verts, mesh.faces[i].numVerts);
  }
}



void freeMesh() {
  for (int i = 0; i < mesh.numFaces; i++) {
    free(mesh.faces[i].verts);
  }
  free(mesh.faces);
}



int main(int argc, char **argv) {
  if (argc == 2 && strcmp(argv[1], "json") == 0) {
    loadMeshFromFile();
  } else {
    loadHouseMesh();
  }
  
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(500, 500);
  (void)glutCreateWindow("Mesh");
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMainLoop();

  freeMesh();
  return 0;
}
