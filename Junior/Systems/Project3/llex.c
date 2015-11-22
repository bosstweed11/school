#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>


struct Node {
	int grade;
	struct Node *next;

};

typedef struct Node NODE;


int main(){
NODE *curr;
NODE *head;

head = NULL;


	int i=0;
	int a;
	while(i<5){
		curr = (NODE *)malloc(sizeof(NODE));
		printf("enter int:\n");
		scanf("%d",&a);
		curr->grade = a;
		curr->next = head;
		head = curr;
		i++;
	}
	
	
	curr = head;
	
	printf("grades\n");
	while(curr){
		printf("%d\n",curr->grade);
		curr = curr->next;
	}


}