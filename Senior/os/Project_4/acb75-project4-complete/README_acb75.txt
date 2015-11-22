Andrew Beers
Project 4
cs1550

Known Issues:
-I have a bug when writing a file larger than one block, the program crashes when i try to fclose my file pointer to .disk
	I cant seem to figure out why this happens, the strange part is that if restart the program, the data on disk is correct
	and the file is supported on multiple blocks, something gets corrupted where the size of the file is not written to the directories
	entry so it doesnt show up on a cat <filename> but i have some debug statements that print out the data from the ./cs1550 program
	
