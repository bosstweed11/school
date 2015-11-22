#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>


typedef struct node NODE;
struct node
{
	int free;
	int data;
	NODE *next;
	NODE *prev;
	
};


NODE *head=NULL;
NODE *tail=NULL;
NODE *curr;

void *my_worstfit_malloc(int size){
	
	curr->free =1;
	curr->data =size;
	curr->next =NULL;
	printf("curr size: %d\n",curr->data);
	
	if(head ==NULL){
		head = curr;
		head->next = tail;
	}
	printf("head size: %d\n",head->data);
	
		
	
	
	
	return sbrk(size +sizeof(NODE)) +sizeof(NODE);


}

void my_free(void *ptr){
	
	//find where ptr is in ll 
	// get size (ptr - sizeof(node))
	// make that node a 'free node' of that size so it can be reallocated
	//if the node is the last node lower sbrk
	// coalesce adjacent nodes if any touching node (prev or next) is also 'free'
	

}

	int main()
	{
		int *ptr_one;
	
		printf("sbrk(0) before : 0x%x\n", sbrk(0));
		ptr_one = (int *)my_worstfit_malloc(sizeof(int));
		printf("sbrk(0) after : 0x%x\n", sbrk(0));
		
		if (ptr_one == 0)
		{
			printf("ERROR: Out of memory\n");
			return 1;
		}

		*ptr_one = 25;
		printf("%d\n", *ptr_one);

		my_free(ptr_one);
		

		return 0;
	}
	