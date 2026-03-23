#include "checkerror.h"
#include <stdio.h>
#include <GL/freeglut_std.h>
#include <GL/gl.h>

void checkError(char *label) {
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    fprintf(stderr, "%s: %s\n", label, gluErrorString(err));
  }
}