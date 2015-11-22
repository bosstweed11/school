#! /usr/bin/python
# This program has been written by Andrew Beers ( acb75 ) for CS 0008 Project 2 6/18/2014
# 
# To run this script: python P2A1_BEERS_3626338.py on the command line, in the directory
# of this file, then proceed to follow the instructions of the program

__author__="beersandrew"
__date__ ="$June 18, 2014 10:43:22 AM$"

import random
import datetime

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

# **activity_three*
# Input: None
# Output: Text
# Description: roll the dice
#
def activity_three():
    return ( random.randint(1,6) + random.randint(1,6) )
    
    

if __name__ == "__main__":

    # Variable to know if the player wants to play again, set initially to start the game loop
    # the first time
    playAgain = 1

    # Variable to say if we are at the comeout roll or not
    comeOutRoll = 1

    # Variable to say if the player has not won or lost yet, set initially to start the game loop
    # the first time
    gameOnGoing = 1

    # Variable to represent the point, -1 initially since it is not set yet
    pointNumber = -1
    
    # Seed the random number generator with the system time
    random.seed( datetime.datetime.now() )
    
    # Function for Activity 1
    activity_one( )

    numPlays = int( input( "How many times should the game be played?\n")
                    

    # Loop while the player wants to keep playing
    while( playAgain == 1 ):

        # Loop while the game is on going
        while( gameOnGoing == 1):
            # Function for Activity 3
            diceRoll = activity_three()

            print ("The roll was: %d\n" % diceRoll)

            if ( ( ( diceRoll == 7 or diceRoll == 11) and comeOutRoll == 1 ) or ( diceRoll == pointNumber and comeOutRoll == 0 ) ):
                print("Player Wins!\n")
                gameOnGoing = 0
            elif( ( ( diceRoll == 2 or diceRoll == 3 or diceRoll == 12 ) and comeOutRoll == 1 ) or ( diceRoll == 7 and comeOutRoll == 0 ) ):
                print("Player Loses!\n")
                gameOnGoing = 0
            elif( comeOutRoll == 1 ):
                print("The point has been set at %d!\n" % diceRoll)
                pointNumber = diceRoll
                comeOutRoll = 0


        playAgain = int( input("Would you like to play again?(1-yes, otherwise-no)\n"))

        if ( playAgain == 1 ):
            comeOutRoll = 1
            pointNumber = -1
            gameOnGoing = 1
        else:
            playAgain = 0
            print("Thanks for playing!\n")

            

        
