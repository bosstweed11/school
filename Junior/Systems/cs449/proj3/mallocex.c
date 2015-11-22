



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


//worst fit malloc
void *my_worstfit_malloc(int size){
	
	curr = (NODE *)sbrk(sizeof(NODE));	//create space for node
	int i=0;
	int node_size=-1;
	int max_size=-1;					//vars for worst fit
	int new=1;
	
	//if there is a linked list parse it for free nodes
	if(head!=NULL){
		temp = head;
		//while the linked list exists
		while(temp !=NULL){
			//if the node is free
			if(temp->free == 2){
				//calculate the size of the node after it would be allocated
				node_size = ((temp->size)-size);
				
				//if this is the greatest amount of leftover space update the max
				if(node_size>max_size){
					worst = temp;
					max_size = node_size;
				}
				
			}
			//traverse through the list
			temp = temp->next;
		}
	}
	
	
	//if the size is greater than the size of a node (sizeof(NODE)= 24) then we need
	//to allocate another node
	if(max_size>=25){
		
		//allocate two nodes
		worst->free =1;
		worst->size =size;
		
		curr = (NODE *)sbrk(sizeof(NODE));		//new node
		
		//if structure for handling the case where the tail is the worst fit.
		if(worst!= tail){	
			//connect the nodes
			curr->next = worst->next;
			curr->prev = worst;
		
			curr->free =2;
			curr->size =max_size-sizeof(NODE);
			(worst->next)->prev= curr;
			
			worst->next = curr;
		}
		else{
			// make curr the tail
			curr->free=2;
			curr->size = max_size-sizeof(NODE);
			worst->next = curr;
			curr->prev = worst;
			curr->next = NULL;
			tail = curr;
			
		}
		
		
		
		
		new =0;	//condition to make a new node
	}
	else if(max_size<25 && max_size>0){
		
		//allocate one node with extra room
		worst->free =1;
		worst->size =size+max_size;
		
		
		
		new=0; //condition to make a new node
	}
	else{
		
		new =1; //create a new node
		
	}
	
	//if the linked list doesn't exist create the first node
	if(head ==NULL){
		curr->free =1;
		curr->size =size;
		
		head = curr;
		tail = curr;
		
		new=1;
	}
	else{
		//if the new node condition is true, create a new node on the list
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
	if(new){
		return sbrk(size);
	}
	else{
		printf("hey\n");
		int return_val = worst;
		return_val+= sizeof(NODE);
		return((void*)return_val);
	}


}

// my free function
void my_free(void *ptr){
	
	curr =(NODE *)(ptr-sizeof(NODE));
	
	//set curr node to free
	curr->free=2;
	
		//if curr isnt the tail, and if the head isnt the tail, proceed
		if((curr !=tail)&&(head!=tail)){
		
			
			//if we dont have to coalesce on both sides
			if(!(((curr->next)->free==2)&&((curr->prev)->free==2))){
				
				//if we coalesce down the link list and the current node is not the head/or the head
				if(((curr->next)->free) ==2&& curr!=head){
					//connect nodes
					(curr->next)->size = (curr->size + (curr->next)->size+sizeof(NODE));
					(curr->prev)->next = curr->next;
					(curr->next)->prev = curr->prev;
				
				}
				else if(((curr->next)->free) ==2&& curr==head){
					//connect nodes
					(curr->next)->size = (curr->size + (curr->next)->size+sizeof(NODE));
					head = curr->next;
					(curr->next)->prev=NULL;
					
				}
		
				//if we want to coalesce up the link list and its not the tail
				if((((curr->prev)->free)==2)&& curr !=head){
					//connect nodes
					(curr->prev)->size = (curr->size +(curr->prev)->size+sizeof(NODE));
					(curr->next)->prev = curr->prev;
					(curr->prev)->next =curr->next;
					
				}
			}
			else{
			
			
				//if the current node is the head just coalesce down
				if(curr==head){
					(curr->next)->size = (curr->size + (curr->next)->size+sizeof(NODE));
					head = curr->next;
					(curr->next)->prev=NULL;
					printf("next coal\n");
				
				}
				else{
					
					//combine sizes
					curr->size = (curr->next)->size + curr->size + (curr->prev)->size + 2*sizeof(NODE);
		
					// if right coalesce is the tail
					if((curr->next)==tail){
						tail = curr;
						curr->next = NULL;
			
					}
					else{
				
						((curr->next)->next)->prev = (curr->next)->prev;
						curr->next = (curr->next)->next;
			
					}
			
					//if left coalesce is the head
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
			//if the head is not the tail and you want to coalesce up
			if(head!=tail){
				if((((curr->prev)->free)==2)&& curr !=head){
					(curr->prev)->size = (curr->size +(curr->prev)->size +sizeof(NODE));
					tail = curr->prev;
					(curr->prev)->next = NULL;
					
				}
			}
		}
	
	
	
	
	
	
	//if the head is the tail and we are freeing it
	if(head == tail){
		sbrk(-(head->size+sizeof(NODE)));
		printf("free it all\n");
		head =NULL;
		tail =NULL;
	}

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
	int *j;

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
	my_free(e);
	my_free(f);
	my_free(g);
	
	
	
	
	
	//my_free(g);
	/*
	my_free(f);

	my_free(e);
	my_free(d);
	my_free(ptr_three);
	my_free(ptr_two);
	my_free(ptr_one);
	*/
	
	//h= (double*)my_worstfit_malloc(sizeof(double));
	//*h=.4;
	j = (int*)my_worstfit_malloc(sizeof(int));
	*j=4;
	my_free(j);
	
	printf("sbrk(0) after : 0x%x\n", sbrk(0));
	//debug info
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
	else{
		if(head!=NULL){
			printf("size: %d\n",head->size);
			printf("avaliable: %d\n",head->free);
		}
	}
	
	
	return 0;
}
