/*
Disegnare un pesce che nuota seguendo un percorso  sinusoidale.
Il punto di vista deve essere posizionato vicino al centro del tracciato del percorso e lo sguardo deve seguire il pesce.
• Caricare l’immagine fish.rgba
• Utilizzare glutIdleFunc per animare il supporto dell’immagine.
*/
#include <stdlib.h>
#include <math.h>
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define NUM_TEXTURES 2

enum {X, Y, Z};

const double upVector[3] = {0.0, 1.0, 0.0};
float camPosition[3] = {0.0, 0.0, 0.0};

const double fishSpeedX = 0.1;
const double fishSpeedZ = 0.1;
const float fishStartX = -30.0;
const float fishEndX = 30.0;
const float fishStartZ = -50.0;
float fishT = 0.0;
float fishPosition[3] = {fishStartX, 0.0, fishStartZ};
GLuint texNames[2];

int currentExercise = 1;

unsigned *read_texture(char *name, int *width, int *height, int *components);

void initTexture (unsigned *image, const GLsizei imageWidth, const GLsizei imageHeight, const unsigned int sWidth, const unsigned int sHeight) {
  unsigned *sImage = (unsigned *)malloc(sWidth * sHeight * 4 * sizeof(unsigned));
   (void)gluScaleImage(
    GL_RGBA, 
    imageWidth, imageHeight,  GL_UNSIGNED_BYTE,   image, 
    sWidth,    sHeight,     GL_UNSIGNED_BYTE,  sImage
  );
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexImage2D(
    GL_TEXTURE_2D, 0, GL_RGBA, 
    sWidth, sHeight, 0, 
    GL_RGBA, GL_UNSIGNED_BYTE, sImage
  );
}

void reshape(int width, int height) {
  if (height == 0)
    height = 1;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(20.0, (double)width / (double)height, 30.0, 100.0);
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
  
  glEnable(GL_LIGHT0);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glPointSize(1.0);
  
  glPushMatrix();
    if (currentExercise == 1) {
      gluLookAt(
        camPosition[X], camPosition[Y], camPosition[Z], 
        fishPosition[X], fishPosition[Y], fishPosition[Z], 
        upVector[X], upVector[Y], upVector[Z]
      );
    } else {
      gluLookAt(
        camPosition[X], camPosition[Y], camPosition[Z], 
        0.0, 0.0, fishStartZ, 
        upVector[X], upVector[Y], upVector[Z]
      );
    }
    glPointSize(1.0);
    
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(1.0, 1.0, 1.0);
    for (int i = 0; i < NUM_TEXTURES; i++) {
      int k = (i == 0) ? 1 : -1;
      glBindTexture(GL_TEXTURE_2D, texNames[i]);
      glPushMatrix();
        glTranslated(k*fishPosition[X], fishPosition[Y], fishPosition[Z]+k);
        glBegin(GL_QUADS);
          glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -0.5, 0.0);
          glTexCoord2f(1.0, 0.0); glVertex3f(1.0,  -0.5, 0.0);
          glTexCoord2f(1.0, 1.0); glVertex3f(1.0,  0.5,  0.0);
          glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 0.5,  0.0);
        glEnd();
      glPopMatrix();
    }
  glPopMatrix();
  glutSwapBuffers();
}

void idle() {
  if (fishPosition[X] > fishEndX) {
    fishPosition[X] = fishStartX;
    fishT = 0.0;
  } else {
    fishPosition[X] += fishSpeedX;
    fishPosition[Z] = fishStartZ + 2*sin(fishT);
    fishT += fishSpeedZ;
  }
  glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case '1':
      currentExercise = 1;
      break;
    case '2':
      currentExercise = 2;
      break;
    default:
      break;
  }
}

int main(int argc, char** argv) {
  const GLsizei sDims = 256;
  char *imageFileNames[NUM_TEXTURES] = {"fish.rgba", "fisha.rgba"}; 
  GLsizei imageWidth[NUM_TEXTURES], imageHeight[NUM_TEXTURES], components[NUM_TEXTURES]; 
  unsigned *image[NUM_TEXTURES];
  for (int i = 0; i < NUM_TEXTURES; i++) {
    image[i] = read_texture(imageFileNames[i], &imageWidth[i], &imageHeight[i], &components[i]); 
  }  
  
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowPosition(10, 10);
  glutInitWindowSize(800, 800);
  (void)glutCreateWindow("TEXTURE");
  
  glGenTextures(NUM_TEXTURES, texNames);
  for (int i = 0; i < NUM_TEXTURES; i++) {
    glBindTexture(GL_TEXTURE_2D, texNames[i]);
    initTexture(image[i], imageWidth[i], imageHeight[i], sDims, sDims); 
  }
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
