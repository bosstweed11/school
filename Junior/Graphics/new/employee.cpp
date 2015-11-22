/*
 *          File: employee.cpp
 *        Author: Robert I. Pitts <rip@cs.bu.edu>
 * Last Modified: April 8, 2000
 *         Topic: Introduction to Inheritance in C++
 * ----------------------------------------------------------------
 *
 * Employee method definitions.
 */


#include <string>
#include <iostream>
#include "employee.h"
using namespace std;


Employee::Employee(std::string theName, float thePayRate)
{
  name = theName;
  payRate = thePayRate;
}

std::string Employee::getName()
{
  return name;
}

float Employee::getPayRate()
{
  return payRate;
}

float Employee::pay(float hoursWorked)
{
  return hoursWorked * payRate;
}

void Employee::setName(std::string theName)
{
	name = theName;
}

void Employee::setPayRate(float thePayRate)
{
	payRate = thePayRate;
}
