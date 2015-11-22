// CoE 449 Assignment 4 e parse Written By Andrew Beers 11/13/2012
//includes
#include <stdio.h>
#include <string.h>	 
// main
int main(int argc, char* argv[]){
	//main vars
	char* temp_string; //arbitrary string length of 100 characters
	int count=0;
	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	int i=0;
	FILE *f;
	count =b-a;
	
	
	if(count<=0||argc!=3){
		
		printf("Invalid Entry\nProgram closing\n");
	
	
	}
	else{
		temp_string = malloc(count*sizeof(int));
		// open the file from command line argument
		f= fopen("/dev/e","r+b");
		// if the file exists proceed
		if(f!=NULL){						//while there are characters to be read
				
				
			fseek(f,a,SEEK_SET);	//set the seek to the offset
				
			fread(temp_string,count,1,f);	// call read on /dev/e to get characters
				
			
			//close file
			fclose(f);
		}
		else{
			//no file found
			printf("sorry could not open file :(\n");
		
		}
		
		while(i<count){
		
			printf("%c",temp_string[i]);	//print out characters
			i++;
		
		}
		printf("\n");
		
	}
	
	return 0;
}
