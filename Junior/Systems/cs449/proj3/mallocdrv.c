#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

//include your cod

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
	
	return sbrk(size);


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
//replace malloc here with the appropriate version of mymalloc
//#define MALLOC malloc
//replace free here with the appropriate version of myfree
//#define FREE free

//You can adjust how many things are allocated
#define TIMES 100

//If you want to make times bigger than 100, remove the call to qsort and do something else.
//Then remove this check.
#if TIMES >= 1000
	#error "TIMES is too big, qsort will switch to mergesort which requires a temporary malloc()ed array"
#endif

//Do not modify below here
struct tree {
	int data;
	struct tree *left;
	struct tree *right;
};

void freetree(struct tree *root)
{
	if(root->left != NULL)
		freetree(root->left);
	if(root->right != NULL)
		freetree(root->right);
	my_free(root);
}

void randominsert(struct tree *head, struct tree *new)
{
	int way;
	struct tree *curr, *prev;
	prev = NULL;
	curr = head;

	while(curr != NULL)
	{
		prev = curr;
		way = rand()%2;
		if(way == 0)
		{
			curr = curr->left;
		}
		else
		{
			curr = curr->right;
		}
	}
	if(way == 0)
		prev->left = new;
	else
		prev->right = new;
}

void printtree(struct tree *head)
{
	struct tree *curr = head;
	if(head == NULL)
		return;

	printtree(curr->left);	
	printf("%d\n", curr->data);
	printtree(curr->right);
}			 

void test1()
{
	int i;
 	
	struct tree *head = (struct tree *)my_worstfit_malloc(sizeof(struct tree));
	head->data = 0;
	head->left = NULL;
	head->right = NULL;

	for(i=1;i<TIMES;i++)
	{
		struct tree *new = (struct tree *)my_worstfit_malloc(sizeof(struct tree));
		new->data = i;
		new->left = NULL;
		new->right = NULL;
		randominsert(head, new);
	}

	printtree(head);
	freetree(head);
}

int comp(const void *a, const void *b)
{
	return *((int *)a) - *((int *)b);
}

void test2()
{
	int *a;
	int i;

	a = (int *)my_worstfit_malloc(TIMES * sizeof(int));

	for(i=0;i<TIMES;i++)
	{
		a[i] = rand()%TIMES + 1;
	}

	qsort(a, TIMES, sizeof(int), comp);

	for(i=0;i<TIMES;i++)
	{
		printf("%d\n", a[i]);
	}

	my_free(a);
	
}

int main() 
{
	srand((unsigned int)time(NULL));
	printf("original val brk: %p\n", sbrk(0));
	test1();
	printf("brk after  test1: %p\n", sbrk(0));
	test2();
	printf("brk after  test2: %p\n", sbrk(0));
	

	return 0;
}




