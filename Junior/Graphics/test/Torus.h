/*
 *          File: manager.h
 *        Author: Robert I. Pitts <rip@cs.bu.edu>
 * Last Modified: April 8, 2000
 *         Topic: Introduction to Inheritance in C++
 * ----------------------------------------------------------------
 *
 * Manager class definition.
 */

#ifndef _TORUS_H
#define _TORUS_H

#include "Shape.h"

class Torus{
public:
  Torus(double r1, double r2, int rs,int vs);
  

protected:
  int rs;
  int vs;
  double r1;
  double r2;
  int type;
 
};

#endif /* not defined _MANAGER_H */