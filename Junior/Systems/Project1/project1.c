//Written by Andrew Beers 9/6/2012 Assignment 1 CoE 449
#include <stdio.h>

int main(){
	
	
	int value;
	int high =3;
	int low=1;
	int comp;
	int scissors;
	int rock;
	int paper;
	int player_score;
	int cpu_score;
	char yes_no[3];
	char choice[8];
	
	//random number seed
	srand((unsigned int)time(NULL));
	
	printf("Welcome to Rock, Paper, Scissors\n\n");
	printf("Would you like to play?");
	scanf("%s",yes_no);
	
	comp = strcmp(yes_no,"yes");
	
	
	//if the player wants to play go into the main loop
	if(comp==0){
	
		//while the player wants to play
		do{
	
			//while either player does not have 3 wins
			do{
				value = rand() % (high - low + 1) + low;
				printf("What is your choice?");
				scanf("%s",choice);
	
				//read in choices
				paper = strcmp(choice,"paper");
				rock = strcmp(choice,"rock");
				scissors=strcmp(choice,"scissors");
	
	
				//scoring structure
				if(value ==1&&paper==0){
					printf("The computer chooses rock. You won this game!\n\n");
					player_score++;
				}
				else if(value==1 &&rock==0){
					printf("The computer chooses rock. Tie!\n\n");
				}
				else if(value==1 &&scissors==0){
					printf("The computer chooses rock. You lost this game!\n\n");
					cpu_score++;
				}	
				else if(value==2 &&paper==0){
					printf("The computer chooses paper. Tie!\n\n");
				}
				else if(value==2 &&rock==0){
					printf("The computer chooses paper. You lost this game!\n\n");
					cpu_score++;
				}
				else if(value==2 &&scissors==0){
					printf("The computer chooses paper. You won this game!\n\n");
					player_score++;
				}
				else if(value==3 &&paper==0){
					printf("The computer chooses scissors. You lost this game!\n\n");
					cpu_score++;
				}
				else if(value==3 &&rock==0){
					printf("The computer chooses scissors. You won this game!\n\n");
					player_score++;
				}
				else if(value==3 &&scissors==0){
					printf("The computer chooses scissors. Tie!\n\n");
				}
				
			printf("The score is now you: %d computer: %d\n\n",player_score,cpu_score);
			}while(!(cpu_score==3||player_score==3));
	
			if(player_score==3){
				printf("Congratulations, you won!\n");
			}
			else{
				printf("Sorry you lost, Try again!\n");
			}

			printf("Would you like to play?");
			scanf("%s",yes_no);
	
			comp = strcmp(yes_no,"yes");
			player_score =0;
			cpu_score=0;
		}while(comp==0);

	}
	

	
	return 0;

}