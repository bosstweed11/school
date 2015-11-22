#! /usr/bin/python
# This program has been written by Andrew Beers ( acb75 ) for CS 0008 Project 3 7/16/2014
# 
# To run this script: python P2A1_BEERS_3626338.py on the command line, in the directory
# of this file, then proceed to follow the instructions of the program

__author__="beersandrew"
__date__ ="$July 16, 2014 10:43:22 AM$"

from random import randrange
from P3A4_BEERS_3626338_WordList import WordList

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

# **check_letter**
# Input: letter, word being guessed, blank word, num strikes, letters left in the blank word
# Output: num strikes, blank word, letters left
# Description: checks how many times a guess is in a word, if at all
    
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

# **restart_game**
# Input: all status variables
# Output: reset status variables
# Description: Allows the user to restart the game with fresh status variables and a new word
    
def restart_game( wordToGuess, lettersLeft, guessedWord, guessedLetters, strikes, words):

    # Get a random word
    wordToGuess = words.getRandomWord()

    # Filling in the blank guessed word string
    guessedWord = [" "] * 2*len(wordToGuess)

    # Reset number of strikes
    strikes = 0

    # Reset number of letters
    guessedLetters = []

    # Set the number of letters in the word to guess
    lettersLeft = len(wordToGuess)

    # Fill in the blanks in the word
    for i in range(2*len(wordToGuess)):
        if ( i % 2 == 0):
            guessedWord[i] = "_"

    return wordToGuess, guessedWord, strikes, guessedLetters, lettersLeft
    
            

# **activity_one**
# Input: array of words
# Output: none
# Description: A customized welcome message within the command console
#              including the users name and description of the program. Followed by the hangman game itself
    
def activity_one( words ):

    # Display the welcome message
    display_welcome()

    # Initialize the hangman state list
    hangmanState = ["Nothing!", "head", "head and torso", "head, torso, and left arm" \
                    , "head, torso, left arm, and right arm",  "head, torso, left arm, right arm, and left leg" \
                    , "head, torso, left arm, right arm, left leg, and right leg"]

    # Declare and Reset all control variables
    wordToGuess = []
    guessedWord = []
    strikes = 0
    guessedLetters = []
    lettersLeft = 0
    wordToGuess, guessedWord, strikes, guessedLetters, lettersLeft = restart_game( wordToGuess, lettersLeft, guessedWord, guessedLetters, strikes, words)

    # Fill in the blank word
    for i in range(2*len(wordToGuess)):
        if ( i % 2 == 0):
            guessedWord[i] = "_"

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
            playing = int(input("Play again? 1- Yes 0- No :"))
    
            while ( playing != 0 and playing != 1 ):
                playing = int(input("Input invalid, Play again? 1- Yes 0- No :"))

            if ( playing == 1 ):
                 # Reset all control variables
                wordToGuess, guessedWord, strikes, guessedLetters, lettersLeft = restart_game( wordToGuess, lettersLeft, guessedWord, guessedLetters, strikes, words)

        # Check to see if we have won, if so handle it
        if ( lettersLeft == 0):
            print("\nCongratulations, you won!")
            print("The word was: %s\n" % "".join(wordToGuess))
            playing = int(input("Play again? 1- Yes 0- No"))
    
            while ( playing != 0 and playing != 1 ):
                playing = int(input("Input invalid, Play again? 1- Yes 0- No :"))

            if ( playing == 1 ):
                 # Reset all control variables
                wordToGuess, guessedWord, strikes, guessedLetters, lettersLeft = restart_game( wordToGuess, lettersLeft, guessedWord, guessedLetters, strikes, words)

# **activity_two**
# Input: none
# Output: array of words
# Description: Load words from a file to be used for the hangman game
    
def activity_two():

    filename = "words.txt"

    file = open(filename,'r');

    return [line.rstrip() for line in file.readlines()]

if __name__ == "__main__":

    # Function for Activity 2
     #   words = activity_two()

    words = WordList()
    words.addWordsFromFile( "words.txt")
    
    # Function for Activity 1
    activity_one( words )        
