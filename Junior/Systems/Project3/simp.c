#include<stdlib.h>
#include<stdio.h>

struct Node {
   int grade;
   struct Node * next;
};

typedef struct Node NODE;

int main() {
   NODE * curr, * head;
   int i;
   int a;
   head = NULL;

   for(i=1;i<=10;i++) {
      curr = (NODE *)malloc(sizeof(NODE));
      printf("hey int\n");
      scanf("%d",&a);
      curr->grade = a;
      curr->next  = head;
      head = curr;
   }

   curr = head;

   while(curr) {
      printf("%d\n", curr->grade);
      curr = curr->next ;
   }
   
}