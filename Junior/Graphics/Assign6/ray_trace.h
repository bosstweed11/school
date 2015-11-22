#ifndef _RAY_TRACE_H_
#define _RAY_TRACE_H_

// platform specific includes
#ifdef __APPLE__
 #include <GLUT/glut.h>
 #include <OpenGL/gl.h>
 #include <OpenGL/glu.h>
#else
 #include <GL/glut.h>
 #include <GL/gl.h>
 #include <GL/glu.h>
#endif

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512

typedef struct color_s
{
	float r, g, b, a;
} color_t;

extern GLuint textureId;
extern color_t screen[WINDOW_WIDTH * WINDOW_HEIGHT];

void plotPixel(color_t *screen, int x, int y, float r, float g, float b, float a);
void set_Rx(GLfloat);
void set_RxT(GLfloat);
void set_Ry(GLfloat);
void set_RyT(GLfloat);
void set_Rz(GLfloat);
void set_RzT(GLfloat);
void set_T(GLfloat,GLfloat,GLfloat);
void set_S(GLfloat sx,GLfloat sy, GLfloat sz);
void matrix_mult(GLfloat [][4],GLfloat []);

#endif