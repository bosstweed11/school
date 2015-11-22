Andrew Beers
CS 1550
Project 2

Known issues:
	- There is a bug in my code where the algorithm will think it is unsafe when it isnt truly unsafe yet
	This is the case in which a thread other than 0 must run to completion fully before any other processes run
	for the current state to still be safe. I have code to handle this case but I do not have the time to debug it and test
	it to see why it is not working. This heart of this code is located at line 256 in banker.c , where i plan to assume i can
	fulfill a process so i act as if i can take its resources to fulfill other processes. I believe this method would work, again
	just don't have the time to test.
	
	- My debug is potentially a little messy, i tried to format it as best I could, so if there are any problems I would have
	no problems coming down to your office to explain anything at all.
	
	