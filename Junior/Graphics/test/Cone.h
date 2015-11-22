/*
 *          File: manager.h
 *        Author: Robert I. Pitts <rip@cs.bu.edu>
 * Last Modified: April 8, 2000
 *         Topic: Introduction to Inheritance in C++
 * ----------------------------------------------------------------
 *
 * Manager class definition.
 */

#ifndef _CONE_H
#define _CONE_H

#include "Shape.h"

class Cone : public Shape {
public:
  Cone(double height, double ray, int rs,
          int vs);
  

protected:
  int rs;
  int vs;
  double ray;
  double height;
  int type;
 
};

#endif /* not defined _MANAGER_H */