//Andrew Beers header file for mymalloc 11/4


void *my_worstfit_malloc(int size);
void my_free(void *ptr);


typedef struct node NODE;
struct node
{
	int free;
	int size;
	NODE *next;
	NODE *prev;
	
};
