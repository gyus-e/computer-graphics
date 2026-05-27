#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

/**
Creare un corridoio con il soffitto bianco, le pareti di mattoni ed il pavimento blu di piastrelle.
• Applicare alle pareti degli applique di tessuto viola con una luce all’interno
• Permettere all’ osservatore di muoversi nel corridoio
*/

#define NUM_TEXTURES 3
enum {FLOOR, WALL, APPLIQUE};
enum {X, Y, Z, W};

const GLfloat red[4] = {1.0, 0.0, 0.0, 1.0};
const GLfloat blue[4] = {0.0, 0.0, 1.0, 1.0};
const GLfloat yellow[4] = {1.0, 1.0, 0.0, 1.0};
const GLfloat black[4] = {0.0, 0.0, 0.0, 1.0};
const GLfloat white[4] = {1.0, 1.0, 1.0, 1.0};
const GLfloat gray[4] = {0.2, 0.2, 0.2, 1.0};
const GLfloat beige[4] = {0.96, 0.96, 0.86, 1.0};

const double corridorDims[3] = {5.0, 5.0, 50.0};

const GLfloat applique1Pos[4] = {-4.0, 1.0, 0.0, 1.0};
const GLfloat applique2Pos[4] = {4.0, 1.0, 0.0, 1.0};

const GLsizei sDims = 256;
char *imageFileNames[NUM_TEXTURES] = {
  "textures/TilesZelligeSquaresWeathered001_COL_VAR1_2K.jpg", 
  "textures/BricksDragfacedRunning008_COL_2K.png",
  "textures/FabricPlainNaturalSheer009_COL_2K.jpg"
}; 
GLsizei imageWidth[NUM_TEXTURES], imageHeight[NUM_TEXTURES], components[NUM_TEXTURES]; 

GLfloat sgenparams[4] = {1, 0, 0, 0}; 
GLfloat tgenparams[4] = {0, 1, 0, 0};
GLuint texNames[NUM_TEXTURES];
unsigned *sImage;


const double step = 1.0;
const double upVector[3] = {0.0, 1.0, 0.0};
double lookAtPoint[3] = {0.0, 0.0, 0.0};
double camPosition[3] = {0.0, 0.0, 30.0};

unsigned *read_texture(char *name, int *width, int *height, int *components);



void initTexture (unsigned *image, const GLsizei imageWidth, const GLsizei imageHeight, const unsigned int sWidth, const unsigned int sHeight) {
  sImage = (unsigned *)malloc(sWidth * sHeight * 4 * sizeof(unsigned));
   (void)gluScaleImage(
    GL_RGBA, 
    imageWidth, imageHeight,  GL_UNSIGNED_BYTE,   image, 
    sWidth,    sHeight,     GL_UNSIGNED_BYTE,  sImage
  );
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  gluBuild2DMipmaps(
    GL_TEXTURE_2D, GL_RGBA, 
    sWidth, sHeight, 
    GL_RGBA, GL_UNSIGNED_BYTE, 
    sImage
  );
}



void errorCallback(GLenum errorCode) {
  const GLubyte *estring;
  estring = gluErrorString(errorCode);
  fprintf(stderr, "Nurbs Error: %s\n", estring);
  exit(0);
}



void reshape(int width, int height) {
  if (height == 0)
    height = 1;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(20.0, (double)width / (double)height, 20.0, 100.0);
  glMatrixMode(GL_MODELVIEW);
}



void display() {
  glClearDepth(1.0);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_TEXTURE_2D);
  // glEnable(GL_TEXTURE_GEN_S);
  // glEnable(GL_TEXTURE_GEN_T);
  
  glLightfv(GL_LIGHT1, GL_POSITION, applique1Pos);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
  glLightfv(GL_LIGHT1, GL_SPECULAR, (GLfloat[]){1.0, 1.0, 1.0, 1.0});
  glEnable(GL_LIGHT1);
  
  glLightfv(GL_LIGHT2, GL_POSITION, applique2Pos);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, white);
  glLightfv(GL_LIGHT2, GL_SPECULAR, (GLfloat[]){1.0, 1.0, 1.0, 1.0});
  glEnable(GL_LIGHT2);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glPointSize(1.0);
  
  glPushMatrix();
    gluLookAt(camPosition[X], camPosition[Y], camPosition[Z], lookAtPoint[X],
                lookAtPoint[Y], lookAtPoint[Z], upVector[X], upVector[Y], upVector[Z]);
 
    // ceiling
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor3fv(white);
    glBegin(GL_QUADS);
      glNormal3f(0.0, -1.0, 0.0);
      glVertex3f(-corridorDims[X], corridorDims[Y], -corridorDims[Z]);
      glVertex3f(-corridorDims[X], corridorDims[Y], corridorDims[Z]);
      glVertex3f(corridorDims[X], corridorDims[Y], corridorDims[Z]);
      glVertex3f(corridorDims[X], corridorDims[Y], -corridorDims[Z]);
    glEnd();

    // walls
    glBindTexture(GL_TEXTURE_2D, texNames[WALL]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    // glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR); 
    // glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    // glTexGenfv(GL_S, GL_OBJECT_PLANE, sgenparams);
    // glTexGenfv(GL_T, GL_OBJECT_PLANE, tgenparams);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3fv(beige);
    glBegin(GL_QUADS);
      glNormal3f(1.0, 0.0, 0.0);
      glTexCoord2f(0.0, 0.0); glVertex3f(-corridorDims[X], -corridorDims[Y], -corridorDims[Z]);
      glTexCoord2f(1.0, 0.0); glVertex3f(-corridorDims[X], -corridorDims[Y], corridorDims[Z]);
      glTexCoord2f(1.0, 1.0); glVertex3f(-corridorDims[X], corridorDims[Y], corridorDims[Z]);
      glTexCoord2f(0.0, 1.0); glVertex3f(-corridorDims[X], corridorDims[Y], -corridorDims[Z]);
    glEnd();
    glBegin(GL_QUADS);
      glNormal3f(-1.0, 0.0, 0.0);
      glTexCoord2f(0.0, 0.0); glVertex3f(corridorDims[X], -corridorDims[Y], -corridorDims[Z]);
      glTexCoord2f(1.0, 0.0); glVertex3f(corridorDims[X], -corridorDims[Y], corridorDims[Z]);
      glTexCoord2f(1.0, 1.0); glVertex3f(corridorDims[X], corridorDims[Y], corridorDims[Z]);
      glTexCoord2f(0.0, 1.0); glVertex3f(corridorDims[X], corridorDims[Y], -corridorDims[Z]);
    glEnd();

        // floor
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColorMaterial(GL_FRONT, GL_SHININESS);
    glColor3fv(blue);
    glBegin(GL_QUADS);
      glNormal3f(0.0, 1.0, 0.0);
      glVertex3f(-corridorDims[X], -corridorDims[Y], -corridorDims[Z]);
      glVertex3f(-corridorDims[X], -corridorDims[Y], corridorDims[Z]);
      glVertex3f(corridorDims[X], -corridorDims[Y], corridorDims[Z]);
      glVertex3f(corridorDims[X], -corridorDims[Y], -corridorDims[Z]);
    glEnd();

    glPushMatrix();
      glTranslatef(applique1Pos[X], applique1Pos[Y], applique1Pos[Z]);
      glRotatef(90, 1.0, 0.0, 0.0);
      glColor3fv(yellow);
      glutSolidCone(0.5, 1, 20, 60);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(applique2Pos[X], applique2Pos[Y], applique2Pos[Z]);
      glRotatef(90, 1.0, 0.0, 0.0);
      glColor3fv(yellow);
      glutSolidCone(0.5, 1, 20, 60);
    glPopMatrix();

  glPopMatrix();
  glutSwapBuffers();
}



void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  // case 27: // ESC
  //   exit(0);
  case 'w':
    camPosition[Z] -= step;
    lookAtPoint[Z] -= step;
    break;
  case 's':
    camPosition[Z] += step;
    lookAtPoint[Z] += step;
    break;
  case 'a':
    camPosition[X] -= step;
    lookAtPoint[X] -= step;
    break;
  case 'd':
    camPosition[X] += step;
    lookAtPoint[X] += step;
    break;
  }
  glutPostRedisplay();
}



int main(int argc, char **argv) {
  unsigned *image[NUM_TEXTURES];
  for (int i = 0; i < NUM_TEXTURES; i++) {
    image[i] = read_texture(imageFileNames[i], &imageWidth[i], &imageHeight[i], &components[i]); 
  }
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowPosition(10, 10);
  glutInitWindowSize(800, 800);
  (void)glutCreateWindow("DOOM 0.1");
  glGenTextures(NUM_TEXTURES, texNames);
  for (int i = 0; i < NUM_TEXTURES; i++) {
    glBindTexture(GL_TEXTURE_2D, texNames[i]);
    initTexture(image[i], imageWidth[i], imageHeight[i], sDims, sDims); 
  }
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
