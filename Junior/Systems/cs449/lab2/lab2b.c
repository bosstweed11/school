#include <stdio.h>

int main(){

	int weight;
	printf("Please enter the weight you'd like to convert: ");
	scanf("%d",&weight);
	
	printf("\n\nHere is our weight on other planets: \n\n");
	
	printf("Mercury \t %4.2f\n",weight*.38);
	printf("Venus \t\t %4.2f\n",weight*.91);
	printf("Mars \t\t %4.2f\n",weight*.38);
	printf("Jupiter \t %4.2f\n",weight*2.54);
	printf("Saturn \t\t %4.2f\n",weight*1.08);
	printf("Uranus \t\t %4.2f\n",weight*.91);
	printf("Neptune \t %4.2f\n",weight*1.19);
	printf("Pluto \t\t %4.2f\n",weight*.06);
	
	return;
	
}
