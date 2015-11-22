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
	int node_size=-1;
	int max_size=-1;
	int new=1;
	printf("ENTER MALLOC\n");
	
	if(head!=NULL){
		temp = head;
		while(temp !=NULL){
			if(temp->free == 2){
				node_size = ((temp->size)-size);
				
				if(node_size>max_size){
					worst = temp;
					max_size = node_size;
				}
				
			}
			temp = temp->next;
		}
	}
	
	printf("%d\n",max_size);
	
	if(max_size>=25){
		printf("hey\n");
		//allocate two nodes
		worst->free =1;
		worst->size =size;
		
		curr = (NODE *)sbrk(sizeof(NODE));
		if(worst!= tail){	
			curr->next = worst->next;
			curr->prev = worst;
		//printf("hey\n");
			curr->free =2;
			curr->size =max_size;
			(worst->next)->prev= curr;
			printf("hey\n");
			worst->next = curr;
		}
		else{
			curr->free=2;
			curr->size = max_size-sizeof(NODE);
			worst->next = curr;
			curr->prev = worst;
			curr->next = NULL;
			tail = curr;
			
		}
		
		
		
		printf("2\n");
		new =0;
	}
	else if(max_size<25 && max_size>0){
		printf("listen\n");
		//allocate one node with extra room
		worst->free =1;
		worst->size =size+max_size;
		
		
		printf("1\n");
		new=0;
	}
	else{
		printf("navi\n");
		new =1;
		printf("0\n");
	}
	
	
	if(head ==NULL){
		curr->free =1;
		curr->size =size;
		printf("curr size: %d\n",curr->size);
		head = curr;
		tail = curr;
		
		
	}
	else{
	
		
		if(new){
		
			curr->free =1;
			curr->size =size;
			tail->next = curr;
			curr->prev = tail;
			if(head==tail){
				tail->prev = head;
			}
			tail = curr;
		}

	}
	
	
	
	
	
	
	printf("EXIT MALLOC\n\n\n");
	
	return sbrk(size);


}

void my_free(void *ptr){
	printf("ENTER FREE\n\n");
	curr =(NODE *)(ptr-sizeof(NODE));
	
	
	curr->free=2;
	
	
	
		if(curr !=tail){
		
		
		
			if(!(((curr->next)->free==2)&&((curr->prev)->free==2))){
			
				if(((curr->next)->free) ==2&& curr!=head){
					(curr->next)->size = (curr->size + (curr->next)->size+sizeof(NODE));
					(curr->prev)->next = curr->next;
					(curr->next)->prev = curr->prev;
					printf("next coal\n");
				}
				else if(((curr->next)->free) ==2&& curr==head){
				
					(curr->next)->size = (curr->size + (curr->next)->size+sizeof(NODE));
					head = curr->next;
					(curr->next)->prev=NULL;
					printf("next coal\n");
			
				}
		
	
				if((((curr->prev)->free)==2)&& curr !=head){
					(curr->prev)->size = (curr->size +(curr->prev)->size+sizeof(NODE));
					(curr->next)->prev = curr->prev;
					(curr->prev)->next =curr->next;
					printf("prev coal\n");
				}
			}
			else{
			
				printf("crazy coal\n");
				
				if(curr==head){
					(curr->next)->size = (curr->size + (curr->next)->size+sizeof(NODE));
					head = curr->next;
					(curr->next)->prev=NULL;
					printf("next coal\n");
				
				}
				else{
				
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
		}
		else{
		
			if((((curr->prev)->free)==2)&& curr !=head){
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
	double *h;

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
	//my_free(ptr_two);
	//my_free(ptr_one);
	
	
	h= (double*)my_worstfit_malloc(sizeof(double));
	*h=.4;
	
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
