#! /usr/bin/python

# To run this script: python P1A4_BEERS_3626338.py on the command line, in the directory
# of this file, then proceed to follow the instructions of the program

__author__="beersandrew"
__date__ ="$May 19, 2014 9:49:22 AM$"

# **activity_one**
# Input: None
# Output: Text
# Description: A customized welcome message within the command console
#              including your name and description of your program.
    
def activity_one( user ):
    print ("\nWelcome, %s." % user)
    print ("Description: This is a budget calculator program.")
    print ("Author: Andrew Beers\n\n")

# **activity_two**
# Input: (global) variables
# Output: Text
# Description: A print out of the variables of the program, their values, and types
#              
    
def activity_two( hourlyRate, overtimeRate, regularHours, overtimeHours, 
                rentAmount, percentEle, percentWater, percentSewer, 
                percentGas, foodBudget, entertainment, carBudget ):
                    
    # Print variables in a nicely formatted way
    print("Here is a dump of all the variables, their values, and their data types:\n")
    print("Variable Name\t : Value\t : Type\n")
    
    print ("hourlyRate\t : " +  repr( hourlyRate) + "\t\t :" + repr( type(hourlyRate) ) )
    print ("overtimeRate\t : " + repr( overtimeRate ) + "\t\t :"+ repr( type(overtimeRate) ) )
    print ("regularHours\t : " + repr( regularHours ) + "\t\t :"+ repr( type(regularHours) ) )
    print ("overtimeHours\t : " + repr( overtimeHours ) + "\t\t :"+ repr( type(overtimeHours) ) )
    print ("rentAmount\t : " + repr( rentAmount ) + "\t\t :"+ repr( type(rentAmount) ) )
    print ("percentEle\t : " + repr( percentEle ) + "\t\t :" + repr( type(percentEle) ) )
    print ("percentWater\t : " + repr( percentWater ) + "\t\t :" + repr( type(percentWater) ) )
    print ("percentSewer\t : " + repr( percentSewer ) + "\t\t :" + repr( type(percentSewer) ) )
    print ("percentGas\t : " + repr( percentGas ) + "\t\t :" + repr( type(percentGas) ) )
    print ("foodBudget\t : " + repr( foodBudget ) + "\t\t :" + repr( type(foodBudget) ) )
    print ("entertainment\t : " + repr( entertainment ) + "\t\t :" + repr( type(entertainment) ) )
    print ("foodBudget\t : " + repr( foodBudget ) + "\t\t :" + repr( type(foodBudget) ) )
    print ("carBudget\t : " + repr( carBudget ) + "\t\t :" + repr( type(carBudget) ) )
    print ("\n")

# **activity_three**
# Input: None
# Output: Text
# Description: A customized welcome message within the command console
#              including your name and description of your program.
    
def activity_three():
    return input('Please enter your name: ')

    
if __name__ == "__main__":
    
    # Function for Activity 3
    username = activity_three()
    
    # Function for Activity 1
    activity_one( username )
    
    # Variable for hourly rate
    hourlyRate = 26
    
    # Variable for overtime rate
    overtimeRate = 30
    
    # Variable for number of regular hours worked
    regularHours = 40
    
    # Variable for overtime hours worked
    overtimeHours = 5
    
    # Variable for the amount rent is
    rentAmount = 600
    
    # Variable for percentage of pay to go towards electric bill
    percentEle = .02
    
    # Variable for percentage of pay to go towards water bill
    percentWater = .02
    
    # Variable for percentage of pay to go towards sewer bill
    percentSewer = .01
    
    # Variable for percentage of pay to go towards gas bill
    percentGas = .1
    
    # Variable for amount of money for the food budget
    foodBudget = 200
    
    # Variable for amount of money for the entertainment budget
    entertainment = 200
    
    # Variable for amount of money for the car budget
    carBudget = 200
    
    
    # Function for Activity 2
    activity_two( hourlyRate, overtimeRate, regularHours, overtimeHours,
                 rentAmount, percentEle, percentWater, percentSewer,
                 percentGas, foodBudget, entertainment, carBudget )
