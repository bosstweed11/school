// CoE 449 Assignment 4 e parse Written By Andrew Beers 11/13/2012
//includes
#include <stdio.h>
#include <string.h>	 
// main
int main(int argc, char* argv[]){
	//main vars
	char test_char[1];
	char temp_string[10000]; //arbitrary string length of 100 characters
	int count=0;
	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	int i=0;
	count =b-a;
	
	FILE *f;
	// open the file from command line argument
	f= fopen("e.txt","r+b");
	// if the file exists proceed
	if(f!=NULL){						//while there are characters to be read
			//read in entire e file to array
			
		fseek(f,a,SEEK_SET);
			
		fread(temp_string,count,1,f);
			
		
		//close file
		fclose(f);
	}
	else{
		//no file found
		printf("sorry could not open file :(\n");
	
	}

	
	
	

	while(i<count){
	
		printf("%c",temp_string[i]);
		i++;

	}
	printf("\n");
	
	
	
	// start of e
	// # of digits
	
	//get from array
	return 0;
}
