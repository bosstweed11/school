//Assignment 3 Systems 449 Andrew Beers malloc free worst fit.
#include "mymalloc.h"
#include <stdio.h>
#include <stdlib.h>



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
