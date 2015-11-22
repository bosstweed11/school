/*
 *          File: employee.h
 *        Author: Robert I. Pitts <rip@cs.bu.edu>
 * Last Modified: April 8, 2000
 *         Topic: Introduction to Inheritance in C++
 * ----------------------------------------------------------------
 *
 * Employee class definition.
 */

#ifndef _EMPLOYEE_H
#define _EMPLOYEE_H

#include <string>

class Employee {
public:
  Employee(std::string theName, float thePayRate);

  std::string getName();
  float getPayRate();
  void setName(std::string theName);
  float pay(float hoursWorked);
  void setPayRate(float thePayRate);

protected:
  std::string name;
  float payRate;
};

#endif /* not defined _EMPLOYEE_H */