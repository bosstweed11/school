Andrew Beers 
acb75@pitt.edu
COE 0147
Assignment 2
Snake
Initialize:
	Plot the initial snake head, body, and tail, as well as 32 randomly placed frogs.
	Initialize body length and time.

Main:
		Start with a polling loop to catch whenever a direction is pressed, this is monitored every 200ms.
		When a direction is pressed, turn the tail off and move the body entries of the snake all down one(last body -> tail, head->first body) and move the head in the direction pressed. Continue this motion to move the snake.
		When the head of the snake is set in the new position, check that position for a frog, if it is, set a flag so that on the next move, the tail does not shut of and the current head is placed as a new body part, increment the body length and continue on with moving the snake.
		When the head eats a frog, a counter is incremented, when this counter gets to 32, the game will end, timer will stop and the game will end with closing comments of the score and game time. The game will also end if the snake head connects with one of the body parts or tail. If this happens the game will end and display the same results as if all the frogs were eaten.

Comments:
The game must be started with a click of the arrows.
The timing begins when the program starts, not when the snake starts moving.
I attempted some extra credit part 1, you can see the code I had for walls, it was pretty close but I didn't have time to fix it entirely.