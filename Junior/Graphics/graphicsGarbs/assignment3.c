//Assignment 3 Systems 449 Andrew Beers malloc free worst fit.

void *my_worstfit_malloc(int size);
void my_free(void *ptr);
int main(){
//call methods
//allocate stuff
//use stuff
//unallocate stuff
//check break ptr

}



void *my_worstfit_malloc(int size){
return sbrk(size +sizeof(struct node)) +sizeof(struct node);


}

void my_free(void *ptr){

	struct node =  ptr -sizeof(node);
	node ->free =1;

}