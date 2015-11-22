//
//  banker.c
//  OS_Project2
//
//  Created by Andy Beers on 5/27/14.
//  Copyright (c) 2014 Andy Beers. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int get_array_size();
int get_num_procs();

// Globals for this part
int *maxResources;
int *availResources;
int *hypAvailResources;
int *canBeFulfilled;
int **currentResources;
int **processMaxResources;

int arraySize = -1;
int numProcs = -1;

/*
	set_max_resources
		Allows banker to know the max resources grid from threads
*/
void set_max_resources( int pid, int resources[] )
{
	int i,j;
	for ( i = 0; i < arraySize; i++ )
		processMaxResources[pid][i] = resources[i];
	return;
}

int* get_curr_resources( int pid )
{
	return currentResources[pid];

}

int* get_max_resources( int pid )
{
	return processMaxResources[pid];

}


void set_available( int resources[] )
{
	int i;
	
	for ( i = 0; i < arraySize; i++ )
	{
		availResources[i] = resources[i];
		hypAvailResources[i] = resources[i];
	}
	
	return;
}


/*
	init_banker
		Initialize the banker's data structures

*/
void init_banker()
{
	int i = 0;
	int j;
	arraySize = get_array_size();
	numProcs = get_num_procs();
	
	// Allocate mem for all resources
	maxResources = malloc( arraySize * sizeof( int ) );
	
	availResources = malloc( arraySize * sizeof( int ) );
	
	hypAvailResources = malloc( arraySize * sizeof( int ) );
	
	currentResources = (int**)malloc( numProcs * sizeof( int* ) );
	
	processMaxResources = (int**)malloc( numProcs  * sizeof( int* ) );
	
	canBeFulfilled = malloc( numProcs * sizeof( int ) );
	
	for( i = 0; i < numProcs; i++ )
	{
		currentResources[i] = (int*)malloc( arraySize * sizeof( int ) );
		
	}
	
	for ( i = 0; i < numProcs; i++ )
	{
		processMaxResources[i] = (int*)malloc( arraySize  * sizeof( int ) );
	
	}
	
	//printf("numProcs from banker %d\n", numProcs);
	for ( i = 0; i < numProcs; i++ )
	{
		
		for( j = 0; j < arraySize; j++ )
		{
			currentResources[i][j] = 0;
			processMaxResources[i][j] = 0;
		}

	}
	
}

/*
	request_resources
		Accepts a resource array thread(pid) would like, checks to see if the state is safe
		after the assignment, either allows a thread to acquire those resources or terminate.

*/
int request_resources( int pid, int resources[] )
{
	int i, j, tryagain = 1;
	int safeResource = 0;
	//printf("\n\nBEGIN REQUEST\n\n");
	//printf("Status\n");
	printf("Process Max resources\n");
	for( i = 0; i < numProcs; i++ )
	{
		printf("Thread %d: ", i );
		for( j = 0; j < arraySize; j++ )
		{
		
			printf("%d\t", processMaxResources[i][j]);
		
		}
		printf("\n");
	}
	
	printf("Process current resources\n");
	for( i = 0; i < numProcs; i++ )
	{
		printf("Thread %d: ", i );
		for( j = 0; j < arraySize; j++ )
		{
		
			printf("%d\t", currentResources[i][j]);
		
		}
		printf("\n");
	}
	
	printf("Available resources\n");
	for( j = 0; j < arraySize; j++ )
	{
		
		printf("%d\t", availResources[j]);
		
	}
	
	printf("\n");
	// Allocation Algorithm
	printf("\nRequest of Resources\nThread %d\n", pid);
	
	printf("\nAllocation request\n");
	for( j = 0; j < arraySize; j++ )
	{
		
		printf("%d\t", resources[j]);
		
	}
	
	for( i = 0; i < arraySize; i++ )
	{
		if ( resources[i] > processMaxResources[pid][i] )
		{
			printf("Error, allocated more than you said you'd need!\n");
			break;
		}
		else if ( resources[i] > availResources[i] )
		{
			printf("Wait!\n");
			break;
		}
		else
		{
			//printf("Resource: %d is safe\n",i);
			safeResource++;
		}
	
	}
	if ( safeResource == arraySize )
	{
		printf("\nAllocation request: Safe\n");
		printf("Remaining resources: \n");
		for ( i = 0; i < arraySize; i++ )
		{
			availResources[i] = availResources[i] - resources[i];
			hypAvailResources[i] = availResources[i];
			currentResources[pid][i] += resources[i];
			printf("%d\t", availResources[i]);
		}
		printf("\n");
	}
	
	
	// Safety Algorithm
	int lost = 0;
	int unsafeProcesses = 0;
	int k;
	int goodResources = 0;
	int safeProcess = 0;
	
	// initialize array that i dont care about... >.<
	for ( i = 0; i < numProcs; i++ )
	{
		canBeFulfilled[i] = 0;
	}
	
	
	// While theres a chance of the system being safe
	// Iterate through the number of processes
	for( i = 0; i < numProcs; i++ )
	{
		// If the process is already safe, move on to the next one
		if ( canBeFulfilled[i] == 0 )
		{
			// Iterate over each resource
			for ( j = 0; j < arraySize; j++ )
			{
				// If there are not enough resources to fulfill this max right now, we lose for this entire process
				if ( currentResources[i][j] +  hypAvailResources[j] < processMaxResources[i][j] )
				{
					lost = 1;
					//printf("Not enough of resource %d for thread %d: lost\n", j, i );
					//printf("Avail: %d\nNeed: %d\n", currentResources[i][j] +  hypAvailResources[j], processMaxResources[i][j]);
					break;
				}
				else
				{
					//printf("Enough of resource %d for thread %d: continuing\n", j, i);
					goodResources++;
				}
			}
			
			// If we can fulfill the process, we will, hypothetically, then release all its resources back in the pool
			if ( goodResources == arraySize )
			{
				safeProcess++;
				canBeFulfilled[i] = 1;
				
				// Code to handle the case where a process id of greater than 0 is required to run
				// first in order for the state to be safe, theres a bug somewhere as it does not work.
				for( j = 0; j < arraySize; j++ )
				{
					hypAvailResources[j] += currentResources[i][j];
					
				}
				
				// If we have all the processes safe, our overall state is safe
				if ( safeProcess == numProcs )
				{
					
					// we need to set the state back to how it was before returning
					for ( k = 0; k < arraySize; k++ )
					{
						hypAvailResources[k] = availResources[k];
					}
					
					for ( k = 0; k < numProcs; k++ )
					{
						canBeFulfilled[k] = 0;
					}
					return 0;
				}
				
				// if we succeeded in fulfilling a process that wasnt the first one, start over to see if we can fulfill it 
				if ( i > 0 )
					i = 0;
			}
			else if ( lost )
			{
				unsafeProcesses++;
			}
			else
			{
				printf("Error with logic\n");
			}
		
			lost = 0;
			goodResources = 0;
		}
		else
		{
			unsafeProcesses++; //we've already checked this one so we want it to count towards not finding one on the current iteration
		}
		
		
	}

	// if all the processes are safe, we can continue
	if ( safeProcess == numProcs )
	{
		tryagain = 0;
		lost = 0;
		
		// we need to set the state back to how it was before returning
		for ( i = 0; i < arraySize; i++ )
		{
			hypAvailResources[i] = availResources[i];
		}
		
		for ( i = 0; i < numProcs; i++ )
		{
			canBeFulfilled[i] = 0;
		}
		return 0;
	}
	else
	{
		printf("we lost :(\n");
		// we need to set the state back to how it was before returning
		printf("Killing process %d.\n", pid );
		
		for ( i = 0; i < arraySize; i++ )
		{
			hypAvailResources[i] = availResources[i];
		}
		return -1;
	
	}
	
	// we need to set the state back to how it was before returning
	for ( i = 0; i < arraySize; i++ )
	{
		hypAvailResources[i] = availResources[i];
	}
	return 0;
}


/*
	release_resources
		Accepts a resource array from a process that will be subtracted from its total
		held resources

*/
void release_resources ( int pid, int resources[] )
{

	int i; 
	
	printf("Releasing Resources:\n");
	printf("Thread %d:\n",pid);
	
	printf("\nRelease request\n");
	
	// Check if the release amount makes sense, then do it
	for( i = 0; i < arraySize; i++ )
	{
		printf("%d\t",resources[i]);
		if ( currentResources[pid] - resources[i] >= 0 )
		{
			currentResources[pid][i] -= resources[i];
			availResources[i] += resources[i];
		}
		
		
	}
	
	printf("\n");
	
	//print out remaining resources
	printf("\nRemaining Resources: \n");
	for( i = 0; i < arraySize; i++ )
	{
		printf("%d\t", availResources[i]);
	}
	printf("\n");
	
	
	return;

}


