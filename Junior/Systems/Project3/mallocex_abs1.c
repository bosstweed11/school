#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>


typedef struct node NODE;
struct node
{
	int free;
	int size;
	NODE *next;
	NODE *prev;
	
};


NODE *head=NULL;
NODE *tail=NULL;
NODE *curr;
NODE *worst;
NODE *temp;



void *my_worstfit_malloc(int size){
	curr = (NODE *)sbrk(sizeof(NODE));
	int i=0;
	int node_size;
	printf("ENTER MALLOC\n");
	
	if(head ==NULL){
		curr->free =1;
		curr->size =size;
		
		printf("curr size: %d\n",curr->size);
		head = curr;
		tail = curr;
		
	}
	else{
	
		
	
		
		curr->free =1;
		curr->size =size;
		tail->next = curr;
		curr->prev = tail;
		if(head==tail){
			tail->prev = head;
		}
		tail = curr;

	}
	/*
	temp = head;
	while(temp !=NULL){
		
		
		
	
	
	}
	*/
	printf("EXIT MALLOC\n\n\n");
	
	return sbrk(size);


}

void my_free(void *ptr){
	printf("ENTER FREE\n\n");
	curr =(NODE *)(ptr-sizeof(NODE));
	
	
	curr->free=0;
	
	
	
		if(curr !=tail){
		
		
		
			if(!(((curr->next)->free==0)&&((curr->prev)->free==0))){
			
				if(((curr->next)->free) ==0&& curr!=head){
					(curr->next)->size = (curr->size + (curr->next)->size+sizeof(NODE));
					(curr->prev)->next = curr->next;
					(curr->next)->prev = curr->prev;
					printf("next coal\n");
				}
				else if(((curr->next)->free) ==0&& curr==head){
				
					(curr->next)->size = (curr->size + (curr->next)->size+sizeof(NODE));
					head = curr->next;
					(curr->next)->prev=NULL;
					printf("next coal\n");
			
				}
		
	
				if((((curr->prev)->free)==0)&& curr !=head){
					(curr->prev)->size = (curr->size +(curr->prev)->size+sizeof(NODE));
					(curr->next)->prev = curr->prev;
					(curr->prev)->next =curr->next;
					printf("prev coal\n");
				}
			}
			else{
			
				printf("crazy coal\n");
				
				curr->size = (curr->next)->size + curr->size + (curr->prev)->size + 2*sizeof(NODE);
		
				if((curr->next)==tail){
					tail = curr;
					curr->next = NULL;
			
				}
				else{
			
					((curr->next)->next)->prev = (curr->next)->prev;
					curr->next = (curr->next)->next;
			
				}
		
				if((curr->prev)==head){
					head = curr;
					curr->prev = NULL;
			
				}
				else{
					((curr->prev)->prev)->next = (curr->prev)->next;
					curr->prev = (curr->prev)->prev;
				}
		
			
			}
		}
		else{
		
			if((((curr->prev)->free)==0)&& curr !=head){
				(curr->prev)->size = (curr->size +(curr->prev)->size +sizeof(NODE));
				tail = curr->prev;
				(curr->prev)->next = NULL;
				printf("prev coal\n");
			}
	
		}
	
	
	
	
	
	
	
	if(head == tail){
		sbrk(-(head->size+sizeof(NODE)));
	}
	
	
	printf("EXIT FREE\n\n\n");
}

int main()
{
	int *ptr_one;
	int *ptr_two;
	double *ptr_three;
	int *d;
	int *e;
	double *f;
	double *g;

	printf("sbrk(0) before : 0x%x\n", sbrk(0));
	ptr_one = (int *)my_worstfit_malloc(sizeof(int));
	
	
	ptr_two = (int *)my_worstfit_malloc(sizeof(int));
	ptr_three = (double*)my_worstfit_malloc(sizeof(double));
	d = (int*)my_worstfit_malloc(sizeof(int));
	e=(int*)my_worstfit_malloc(sizeof(int));
	f=(double*)my_worstfit_malloc(sizeof(double));
	g = (double*)my_worstfit_malloc(sizeof(double));
	printf("sbrk(0) after : 0x%x\n", sbrk(0));
	if (ptr_one == 0)
	{
		printf("ERROR: Out of memory\n");
		return 1;
	}
	*ptr_one = 25;		
	*ptr_two = 40;
	*ptr_three=1.434;
	*d = 5;
	*e = 2;
	*f = .22;
	*g = 30.1;
	printf("%d\n", *ptr_one);
	printf("%d\n", *ptr_two);
	printf("%f\n", *ptr_three);
	printf("%d\n", *d);
	printf("%d\n", *e);
	printf("%f\n", *f);
	printf("%f\n", *g);
	
	//my_free(ptr_one);
	//my_free(ptr_two);
	//my_free(ptr_three);
	//my_free(d);
	//my_free(e);
	//my_free(f);
	//my_free(g);
	
	
	
	
	
	my_free(g);
	
	my_free(f);
	
	my_free(e);
	my_free(d);
	my_free(ptr_three);
	my_free(ptr_two);
	//my_free(ptr_one);
	
	
	printf("sbrk(0) after : 0x%x\n", sbrk(0));
	
	if(head!=tail){
		curr= tail;
		while(curr!=head){
			printf("size: %d\n",curr->size);
			printf("avaliable: %d\n",curr->free);
			curr = curr->prev;
		}
		printf("size: %d\n",curr->size);
		printf("avaliable: %d\n",curr->free);
		printf("front to back\n\n\n");
		curr= head;
		while(curr!=NULL){
			printf("size: %d\n",curr->size);
			printf("avaliable: %d\n",curr->free);
			curr = curr->next;
		}
	}
	
	//printf("hey");
	return 0;
}
