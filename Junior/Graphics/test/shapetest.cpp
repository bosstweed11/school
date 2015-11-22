/*
 *          File: empltest.cpp
 *        Author: Robert I. Pitts <rip@cs.bu.edu>
 * Last Modified: April 8, 2000
 *         Topic: Introduction to Inheritance in C++
 * ----------------------------------------------------------------
 *
 * OVERVIEW:
 * =========
 * This program illustrates the creation and use of employee objects.
 */

#include <iostream>
#include <glut/glut.h>
#include "Shape.h"
#include "Cylinder.cpp"
#include "Sphere.h"
#include "Torus.h"
#include "Cone.h"
#include "Shape.cpp"


#include <stdio.h>

//#include "supervisor.h"

 using namespace std; 

int main()
{
	int k=5;
 Cylinder* cyl = new Cylinder(1,1,1,1);
  
  printf("k = %d\n",k);
  k = (*cyl).get_type();
  
  printf("k = %d\n",k);
  (*cyl).Create();




  return 0;
}