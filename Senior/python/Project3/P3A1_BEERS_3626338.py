#! /usr/bin/python
# This program has been written by Andrew Beers ( acb75 ) for CS 0008 Project 3 7/16/2014
# 
# To run this script: python P2A1_BEERS_3626338.py on the command line, in the directory
# of this file, then proceed to follow the instructions of the program

__author__="beersandrew"
__date__ ="$July 16, 2014 10:43:22 AM$"

# **display_welcome**
# Input: None
# Output: Text
# Description: A welcome message within the console
#            that gives a detailed description of the program.
    
def display_welcome():
    # Display the welcome message
    print ("\nWelcome to Hangman!")
    print ("Description: This is a program to simulate a game of Hangman.")
    print ("Rules: The game starts with a blank word, the player guesses a letter they. \n \
    think is in the word. If the letter is in the word, the letter wil be displayed in the blank word.\n \
    If the letter is not in the word then the player gets a strike.\n \
    The player wins if the entire word is correctly guessed, \n \
    and loses if the player gets 6 strikes.")
    print ("Author: Andrew Beers\n\n")

# **invalid_guess**
# Input: string letter
# Output: int valid
# Description: Determines whether the letter guessed was a valid hangman letter
    
def invalid_guess( letter, guessedWord, guessedLetters ):
    if ( letter.isalpha() ):
        if ( letter not in guessedWord and letter not in guessedLetters ):
            return 0
        else:
            print("You've guessed this letter already!\n")
    else:
        print("That was not a valid letter.\n")
        
    return 1

def check_letter( letter, guessedWord, wordToGuess, strikes, lettersLeft):

    # Initialize a variable to indicate we've had any matches of the current letter
    found = 0
    
    # Iterate over the word we are trying to guess
    for i in range(len(wordToGuess)):

        # If the index we are on contains the letter we are looking for, add that letter to
        # the blank word
        if ( wordToGuess[i] == letter ):
            guessedWord[i*2] = letter
            found = 1
            lettersLeft -= 1

    if ( found == 0):
        strikes += 1

    return strikes, guessedWord, lettersLeft
            

# **activity_one**
# Input: None
# Output: Text
# Description: A customized welcome message within the command console
#              including the users name and description of the program.
    
def activity_one( ):

    # Display the welcome message
    display_welcome()

    # Initialize the hangman state list
    hangmanState = ["Nothing!", "head", "head and torso", "head, torso, and left arm" \
                    , "head, torso, left arm, and right arm",  "head, torso, left arm, right arm, and left leg" \
                    , "head, torso, left arm, right arm, left leg, and right leg"]

    # Set the word we want to guess
    wordToGuess = list("andrew")

    # Find out how many correct guesses we need
    lettersLeft = len(wordToGuess)

    # Set the blank word to be displayed
    guessedWord = [" "] * 2*len(wordToGuess)

    # Create the guessedLetters list.
    guessedLetters = []

    # Set the number of initial strikes the player has
    strikes = 0

    # Fill in the blank word
    for i in range(2*len(wordToGuess)):
        if ( i % 2 == 0):
            guessedWord[i] = "_"

    print("The word we are trying to guess is %s\n" % "".join(wordToGuess))
    print("The blank word is %s\n" % "".join(guessedWord))

    # Set the control variable of the program
    playing = 1

    # While the game is going on, lets play
    while( playing == 1):
        
        # Display the hangman state
        print("Hangman has : %s\n" % "".join(hangmanState[strikes]) )

        # Display the letters that have already been guessed
        print("Previous guesses: %s\n" % "".join(guessedLetters) )

        # Display the current state of the word
        print("Word : %s\n" % "".join(guessedWord))

        # Prompt the user for a letter
        letter = input("Which letter would you like to guess? ")[0].lower()

        # If the letter is invalid, make the user prompt a valid letter
        while( invalid_guess(letter, guessedWord, guessedLetters)):
            letter = input("Please guess a valid letter: ")[0].lower()

        # Once we have a valid letter we can put it in our guessed pool
        guessedLetters.append(letter)

        # Check if the letter is in the word
        strikes, guessedWord, lettersLeft = check_letter( letter, guessedWord, wordToGuess, strikes, lettersLeft)

        # Check to see if we have lost, if so handle it
        if ( strikes == 6 ):
            print("\nSorry you lost!")
            print("The word was: %s\n" % "".join(wordToGuess))
            playing = 0

        # Check to see if we have won, if so handle it
        if ( lettersLeft == 0):
            print("\nCongratulations, you won!")
            print("The word was: %s\n" % "".join(wordToGuess))
            playing = 0
       

if __name__ == "__main__":
    
    # Function for Activity 1
    activity_one( )        
