/*
 *          File: manager.h
 *        Author: Robert I. Pitts <rip@cs.bu.edu>
 * Last Modified: April 8, 2000
 *         Topic: Introduction to Inheritance in C++
 * ----------------------------------------------------------------
 *
 * Manager class definition.
 */

#ifndef _CYLINDER_H
#define _CYLINDER_H

#include "Shape.h"

class Cylinder : public Shape{


public:
 Cylinder(double height,double ray, int rs, int vs):Shape(vs){};
 void Create();

protected:
  int rs;
  int vs;
  double height;
  double ray;
  int type;
};

#endif /* not defined _MANAGER_H */