/*
 *          File: manager.h
 *        Author: Robert I. Pitts <rip@cs.bu.edu>
 * Last Modified: April 8, 2000
 *         Topic: Introduction to Inheritance in C++
 * ----------------------------------------------------------------
 *
 * Manager class definition.
 */

#ifndef _SPHERE_H
#define _SPHERE_H

#include "Shape.h"

class Sphere : public Shape {
public:
  Sphere(double ray, int rs,
          int vs);
  

protected:
  int rs;
  int vs;
  double ray;
  int type;
 
};

#endif /* not defined _MANAGER_H */