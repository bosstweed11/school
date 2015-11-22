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

#include "Shape.cpp"
using namespace std; 

int main()
{
    int k=5;
    Shape* cyl = new Shape(1,1,1,20,20);
    Shape* sph = new Shape(2,1,1,20,20);
    Shape* con = new Shape(3,1,1,20,20);
    Shape* tor = new Shape(4,1,1,20,20);
  
  
    printf("k = %d\n",k);
    k = (*cyl).get_rs();
  
    printf("k = %d\n",k);
    (*cyl).set_rs(12);
    k = (*cyl).get_rs();
  
    printf("k = %d\n",k);
	
	
	(*cyl).show_verts();
  

  return 0;
}