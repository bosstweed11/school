#include<stdio.h>


int fib(int);

int main(void)
{
	int num;
	printf("Enter the number of the Fibonacci sequence you would like: ");
	scanf("%d",&num);
	
	printf("The %dth number is %d.\n",num,fib(num));


}


int fib(int a)
{
	int number;
	
	if(a==0)
	{
		return 0;
	}
	else if(a==1)
	{
		return 1;
	}
	else
	{
		return(fib(a-2)+fib(a-1));
	}
}
	
	
	
