#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include "tinyxml2.h"
#include <string>
#include "arena.h"
#include "shot.h"
#include "imageloader.h"

void RasterChars(GLfloat x, GLfloat y, GLfloat z, const char* text, double r, double g, double b);
void PrintTextUI(GLfloat x, GLfloat y, const char* text, double r, double g, double b);
void PrintTextWorld(GLfloat x, GLfloat y, GLfloat z, const char* text, double r, double g, double b);