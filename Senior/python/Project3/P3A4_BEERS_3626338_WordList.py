#! /usr/bin/python
# This program has been written by Andrew Beers ( acb75 ) for CS 0008 Project 3 7/16/2014
# 
# To run this script: This is a module not meant to be run independently

__author__="beersandrew"
__date__ ="$July 16, 2014 10:43:22 AM$"

from random import randrange

class WordList():


    def __init__(self):
        self._wordList = None

     # **addWord**
    # Input: word to add to the word list
    # Output: none
    # Description: Adds a new word to the word list
    def addWord( self, newWord ):
        self._wordList.append(newWord.rstrip().lower())
        return

     # **addWordsFromFile**
    # Input: filename
    # Output: none
    # Description: populates the internal wordlist object with the lines of the given file
    def addWordsFromFile( self, filename ):

        file = open(filename,'r');

        self._wordList = [line.rstrip() for line in file.readlines()]

        return

     # **getRandomWord**
    # Input: none
    # Output: a random word from the word list
    # Description: Gives the user a random word from the wordlist
    
    def getRandomWord(self):

        return self._wordList[randrange(self.numWords())]

    
    # **numWords**
    # Input: none
    # Output: number of words in the wordlist
    # Description: Gives the user the size of the word list
    
    def numWords(self):

        return len(self._wordList)

