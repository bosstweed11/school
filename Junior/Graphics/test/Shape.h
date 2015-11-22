//Andrew Beers shape.h

#ifndef _SHAPE_H
#define _SHAPE_H

#include <stdlib.h>

#define _USE_MATH_DEFINES //cause vs is dumb :-(
#include <math.h>
#include <glut/glut.h>

#ifndef min //In VC++ 2008, Ryan got a warning about min redefinition, so let's not redefine it if something else already did; sigh
#define min(a,b) ((a) < (b)? a:b)
#endif


class Shape {
public:
  Shape(int type);
  
  void set_Rx(GLfloat theta);
  void set_Ry(GLfloat theta);
  void set_Rz(GLfloat theta);
  void set_T(GLfloat tx,GLfloat ty, GLfloat tz);
  void set_S(GLfloat sx,GLfloat sy, GLfloat sz);
  void matrix_mult(GLfloat M[][4],GLfloat V[]);
  int get_type();
 

protected:
  int shape_type;
  GLfloat vertices[50][50][4];
  GLfloat v_start[4];
   GLfloat result[4];
    GLfloat result2[4];
  double Rz[4][4];
  double Ry[4][4];
  double Rx[4][4];
  double S[4][4];
  double T[4][4];
};

#endif /* not defined _EMPLOYEE_H */