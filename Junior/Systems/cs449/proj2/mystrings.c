// CoE 449 Assignment 2 mystrings Written By Andrew Beers 10/4/2012
//includes
#include <stdio.h>
#include <string.h>	 
// main
int main(int argc, char* argv[]){
	//main vars
	char test_char[1];
	char temp_string[100]; //arbitrary string length of 100 characters
	int count=0;
	FILE *f;
	// open the file from command line argument
	f= fopen(argv[1],"r+b");
	// if the file exists proceed
	if(f!=NULL){
	
		while(!feof(f)){							//while there are characters to be read
			//read in first char
			fread(test_char,1,1,f);
			if(*test_char>=32&&*test_char<=126){		//test if char is within readable ASCII ranges
				do{
					strncat(temp_string,test_char,1);	//if it is append to the current string 
					count++;						//increment counter
					fread(test_char,1,1,f);			//read in the next value
					
				}while(*test_char>=32&&*test_char<=126&&count<99);	//repeat process
				
				if(count>=4){						//if you have 4 or more characters print out the string
					strcpy(temp_string,temp_string);			//append null char
					printf("%s\n",temp_string);		//print string
				
				}
				strncpy(temp_string,"",1);				//set temp_string to 0
				count=0;							//set counter to 0
			}
		}
		//close file
		fclose(f);
	}
	else{
		//no file found
		printf("sorry could not open file :(\n");
	
	}
	return 0;
}
