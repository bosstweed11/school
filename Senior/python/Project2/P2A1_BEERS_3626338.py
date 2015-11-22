#! /usr/bin/python
# This program has been written by Andrew Beers ( acb75 ) for CS 0008 Project 2 6/18/2014
# 
# To run this script: python P2A1_BEERS_3626338.py on the command line, in the directory
# of this file, then proceed to follow the instructions of the program

__author__="beersandrew"
__date__ ="$June 18, 2014 10:43:22 AM$"

# **activity_one**
# Input: None
# Output: Text
# Description: A customized welcome message within the command console
#              including the users name and description of the program.
    
def activity_one( ):
    # Prompt for the user's name
    user = input("Please enter your name: ");

    # Display the welcome message
    print ("\nWelcome, %s." % user)
    print ("Description: This is a program to simulate a game of Craps.")
    print ("Rules: The game starts with the first roll, the come out roll. The player wins on a 7 or 11.\n \
    and loses on a 2, 3, or 12. \n \
    Any other number becomes the point number.\n \
    The player continues their roll until the player rolls the point number again and wins. \n \
    The player can also roll a 7 and lose, where the game goes back to the come out roll.")
    print ("Author: Andrew Beers\n\n")

if __name__ == "__main__":
    
    # Function for Activity 1
    activity_one( )

