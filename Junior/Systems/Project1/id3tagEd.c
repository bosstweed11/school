// CoE 449 Assignment 1 Written By Andrew Beers 9/21 2012

//includes
#include <stdio.h>
#include <string.h>


		 
// main
int main(int argc, char* argv[]){
	
	//main vars
	int index;
	char temp[5];
	int tagpresent;
	int number;
	
	// struct for id3tag
	// each component given its number of bytes specified
	struct ID3tag{
		char test[3];
		char song[30];
		char artist[30];
		char album[30];
		char year[4];
		char comment[28];
		char empty_bit;
		char track_number;
		char genre;
		
	
	}tag;
	
	FILE *f;
	// open the file from command line argument
	f= fopen(argv[1],"r+b");
	
	// if the file exists proceed with looking for the tag
	if(f!=NULL){
	
		//check to see if the tag is in the beginning of the file
		fread(tag.test,3,1,f);
		if(strcmp(tag.test,"TAG")==0){
			//printf("beginning of file tag\n\n");
			tagpresent=1;
		}
		else{
	
			
			//check to see if the tag is at the end of the file
			fseek(f,-128,SEEK_END);
			fread(tag.test,3,1,f);
			
			
			if(strcmp(tag.test,"TAG")==0){
				fseek(f,-3,SEEK_CUR);
				fread(&tag,sizeof(tag),1,f);
				tagpresent=1;
			}
			else{
				
				tagpresent=0;
				
			}	
		}
		
		if(argc ==2){
			if(tagpresent==1){
				//Solution for printing out the year without any extra digits
				temp[0] = tag.year[0];
				temp[1] = tag.year[1];
				temp[2] = tag.year[2];
				temp[3] = tag.year[3];
				temp[4] = '\0';
				//number = atoi(tag.track_number); // my attempt at parsing the track number in 
				
			
			// print out formatted tag information
				printf("Title: %s\n",tag.song);
				printf("Artist: %s\n",tag.artist);
				printf("Album: %s\n",tag.album);
				printf("Year: %s\n",temp); 
				//printf("Track Number: %d\n\n",number); // this attempt gave me a Segmentation fault:11
				printf("Track Number: %c\n\n",tag.track_number);
				printf("Comment: %s\n",tag.comment);
			}
			else{
				printf("Sorry no tag was detected on this file.\n");
			}
			
			
		}
		else if(argc>2){
			index =2;
			
			//if there is no tag, create one
			if(tagpresent==0){
				strcpy(tag.test,"TAG");
				fseek(f,125,SEEK_CUR);
				tag.song[0]='\0';
				tag.artist[0]='\0';
				tag.album[0]='\0';
				tag.year[0]='\0';
				tag.track_number='\0';
				tag.comment[0]='\0';
				fwrite(&tag,sizeof(tag),1,f);
			}
			
			
			
			//loop to handle all command line field value changes
			do{
				if(strcmp(argv[index],"-title")==0){
					
					index++;
					printf("The title has been changed.\n");
					strncpy(tag.song,argv[index],sizeof(tag.song));
					tag.song[29]='\0';
					
					
				}
				else if(strcmp(argv[index],"-artist")==0){
					index++;
					printf("The artist has been changed.\n");
					strncpy(tag.artist,argv[index],sizeof(tag.artist));
					tag.artist[29]='\0';
					
					
				}
				else if(strcmp(argv[index],"-album")==0){
					index++;
					printf("The album has been changed.\n");
					strncpy(tag.album,argv[index],sizeof(tag.album));
					tag.album[29]='\0';
					
				}
				else if(strcmp(argv[index],"-track")==0){
					index++;
					printf("The track number has been changed.\n");
					tag.track_number=*argv[index];
				
					
				}
				else if(strcmp(argv[index],"-year")==0){
					index++;
					printf("The year has been changed.\n");
					strncpy(tag.year,argv[index],sizeof(tag.year));
					
				}
				else if(strcmp(argv[index],"-comment")==0){
					index++;
					printf("The comment has been changed.\n");
					strncpy(tag.comment,argv[index],sizeof(tag.comment));
					tag.song[27]='\0';
					
					
				}
				index++;
			
			}while(index<argc);
			
			
			//write the tag to the file
			fseek(f,-128,SEEK_END);
			fwrite(&tag,sizeof(tag),1,f);
			
			
		}
		else{
			printf("Please enter a song file as a command line argument.\n");
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