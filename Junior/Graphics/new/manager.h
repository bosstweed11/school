/*
 *          File: manager.h
 *        Author: Robert I. Pitts <rip@cs.bu.edu>
 * Last Modified: April 8, 2000
 *         Topic: Introduction to Inheritance in C++
 * ----------------------------------------------------------------
 *
 * Manager class definition.
 */

#ifndef _MANAGER_H
#define _MANAGER_H

#include "employee.h"

class Manager : public Employee {
public:
  Manager(std::string theName,
          float thePayRate,
          bool isSalaried);

  bool getSalaried();
  void setSalaried(bool isSalaried);
  float pay(float hoursWorked);
  

protected:
  bool salaried;
};

#endif /* not defined _MANAGER_H */