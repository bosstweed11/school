Andrew Beers acb75@pitt.edu

My assignment 1 was done with the following logic:
1) make a initial loop introducing the program, getting the computer word and filling in all registers that would be used in one round of the program.
2) after the program is run once, theres a main loop it will run through this loop for the life of the program now
	a) once in the main loop it will read off the appropriate text of the current word that is being guessed at and give an option for input, ( any letter, or ? or . or !)
	b) if the user enters a letter it will compare to all indices of the current word being guessed at to see if there are matches, if so theres a correct letter, else decrement round score
	c) if the user enters !, he will be prompted to enter an entire guess at the word if correct, they win doubling the round score, else lose
	d) if the user enters ., it is a forfeit and they will be asked if they want to play again
	e) if the user enters ?, the first non underscore letter will be shown
3) once the round is finished the player will chose to play again or to end the game
	a) if the game is ended then show score and end
	b) if not repeat 2)
	
so there are 2 main loops of continuing the game or guessing at the word, with an initial loop beginning the program.


Bugs: There are no bugs in my program that I am aware of, I have tested for all of the requirements of the program and it seems correct to me.