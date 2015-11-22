/*
 *          File: manager.cpp
 *        Author: Robert I. Pitts <rip@cs.bu.edu>
 * Last Modified: April 8, 2000
 *         Topic: Introduction to Inheritance in C++
 * ----------------------------------------------------------------
 *
 * Manager method definitions.
 */

#include "manager.h"

using namespace std;

Manager::Manager(std::string theName,
                 float thePayRate,
                 bool isSalaried)
  : Employee(theName, thePayRate)
{
  salaried = isSalaried;
}

bool Manager::getSalaried()
{
  return salaried;
}

float Manager::pay(float hoursWorked)
{
  if (salaried)
    return payRate;
  /* else */
  return Employee::pay(hoursWorked);
}
void Manager::setSalaried(bool isSalaried)
{
	salaried = isSalaried;
}