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

# **activity_four**
# Input: None
# Output: All global variable assignment
# Description: Allow the user to assign values to all variables
    
def activity_four():
    
    # Block to accept input for variables
    
    # Variable for hourly rate
    hourlyRate = int( input("Enter your hourly rate: ") )
    
    # Variable for overtime rate
    overtimeRate = int( input("Enter your overtime rate: "))
    
    # Variable for number of regular hours worked
    regularHours = int( input("Enter the amount of regular hours worked: "))
    
    # Variable for overtime hours worked
    overtimeHours = int( input("Enter the amount of overtime hours worked: "))
    
    # Variable for the amount rent is
    rentAmount = float( input("Enter the price of rent: "))
    
    # Variable for percentage of pay to go towards electric bill
    percentEle = float( input("Enter the percentage of pay to go towards electric: "))
    
    # Variable for percentage of pay to go towards water bill
    percentWater = float( input("Enter the percentage of pay to go towards water: "))
    
    # Variable for percentage of pay to go towards sewer bill
    percentSewer = float( input("Enter the percentage of pay to go towards sewer: "))
    
    # Variable for percentage of pay to go towards gas bill
    percentGas = float( input("Enter the percentage of pay to go towards gas: "))
    
    # Variable for amount of money for the food budget
    foodBudget =int( input("Enter your food budget: "))
    
    # Variable for amount of money for the entertainment budget
    entertainment = int( input("Enter your entertainment budget: ") )
    
    # Variable for amount of money for the car budget
    carBudget = int( input("Enter your car budget: ") )
    
    # Function for Activity 2
    activity_two( hourlyRate, overtimeRate, regularHours, overtimeHours, 
                rentAmount, percentEle, percentWater, percentSewer, 
                percentGas, foodBudget, entertainment, carBudget )
                
    activity_five( hourlyRate, overtimeRate, regularHours, overtimeHours, 
                rentAmount, percentEle, percentWater, percentSewer, 
                percentGas, foodBudget, entertainment, carBudget)
                
# **activity_five**
# Input: Variables
# Output: All global variable assignment
# Description: Output of persons monthly expenses, sum of all deductions, gross pay, and net pay          
def activity_five(hourlyRate, overtimeRate, regularHours, overtimeHours, 
                rentAmount, percentEle, percentWater, percentSewer, 
                percentGas, foodBudget, entertainment, carBudget):
    
    # Variable to store the gross pay the user made
    grossPay = hourlyRate * regularHours + overtimeRate * overtimeHours
    
    # Variable to store the amount of money spent on electric each month
    monthlyElectric = percentEle * grossPay
    
    # Variable to store the amount of money spent on water each month
    monthlyWater = percentWater * grossPay
    
    # Variable to store the amount of money spent on sewer each month
    monthlySewer = percentSewer * grossPay
    
    # Variable to store the amount of money spent on gas each month
    monthlyGas = percentGas * grossPay
    
    # Variable to store the total deductions
    totalDeductions = monthlyElectric + monthlyWater + monthlySewer + monthlyGas + foodBudget + entertainment + carBudget # + rentAmount?
    
    # Variable to store the net pay of the user for the month
    netPay = grossPay - totalDeductions
    
    
    # Print out section
    
    
    # Print out monthly expenses
    print("Monthly expenses: ")
    print("Electric: \t%.2f" % monthlyElectric )
    print("Water: \t%.2f" % monthlyWater )
    print("Sewer: \t%.2f" % monthlySewer )
    print("Gas: \t%.2f\n" % monthlyGas )

    
    # Print out deductions
    print("Total Deductions: \t%.2f \n" % totalDeductions)
    
    # Print out gross pay
    print("Gross pay: \t%.2f \n" % grossPay )
    
    #Print out net pay
    print("Net pay: \t%.2f \n" % netPay )
    
if __name__ == "__main__":
    
    # Function for Activity 3
    username = activity_three()
    
    # Function for Activity 1
    activity_one( username )
    
    # Function for Activity 4
    activity_four()
