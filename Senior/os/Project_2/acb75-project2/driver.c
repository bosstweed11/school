//
//  driver.c
//  OS_Project2
//
//  Created by Andy Beers on 5/27/14.
//  Copyright (c) 2014 Andy Beers. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>


// Globals that will be used as getters from the threads
int sizeOfArray = 0;
int numberOfProcesses = 0;
int id = 0;


void init_banker();
void set_available();



void *take_resources( void *resources_void_ptr );


// do i need these here?
void release_resources ( int pid, int resources[] );

int request_resources( int pid, int resources[] );

pthread_t *threadArray;

int* readyArray;
int doneProcs = 0;


// Barrier set for ending processes
void set_ready( int a, int pid )
{
	readyArray[pid] = a;
}

// increment done processes so we can know when we can quit
void set_done ( int a, int pid )
{
	doneProcs++;
}

int get_done()
{
	if( doneProcs == numberOfProcesses - 1 )
		return 1;
	else
		return 0;
}

int get_ready()
{
	int i;
	for( i = 0; i < numberOfProcesses; i++ )
	{
		if ( readyArray[i] == 0)
		{
			//printf("thread %d is not ready!\n",i);
			return 0;
		}
	
	}
	return 1;
}

/*
	get_array_size
		Provides a way for the threads to know how many elements the resources array contains
*/
int get_array_size()
{
	return sizeOfArray;
}

/*
	get_num_procs
		Provides a way for the banker to know how many processes there are
*/
int get_num_procs()
{
	return numberOfProcesses;
}


/*
	get_thread_id
		Provides a way to grant each thread a unique id, assumption of not having 2^32 + 1 threads....
*/
int get_thread_id()
{
	return id++;

}

/*
	kick_off_processes
		Kicks off the appropriate amount of 'processes' as new threads, giving them 
		each the max resources array

*/
void kick_off_processes ( int numProcesses, int *resources )
{
	int i = 0;
	threadArray = (pthread_t *) malloc ( numProcesses * sizeof(pthread_t) );

	if ( threadArray == NULL )
	{
	
		printf("Error out of memory\n");
		exit(0);
	}

	printf("Creating %d Processes.\n", numProcesses );
	for ( i = 0; i < numProcesses; i++ )
	{
		if( pthread_create( &threadArray[i], NULL, take_resources, (void*) resources ) ) 
		{

			fprintf(stderr, "Error creating thread\n");
			exit(0);

		}
	}
}

/*
	run_bankers
		the driver will run the bankers...? not sure about this yet

*/
void run_bankers ( int numProcesses, int *resources )
{
	printf("Running the algo\n");
	
	while ( doneProcs < numProcesses )
	{
	
		// when we are out of processes we'll die

		
	}


}

/*
	input_error
		prints to the screen that we've seen an invalid command line argument list

*/
void input_error ()
{
	printf("Format: ./bankers –n <numprocs> -a <available vector>\n");
}

int main(int argc, const char * argv[])
{
	// num processes
	int numProcs = 0;
	int *resources, *temp;
	char broken = 0;
	
	// Allocate space for the resources array
	resources = malloc( sizeof( int ) );
	
	// Process command line args
	if ( argc > 1 )
	{
		if ( strcmp(argv[1], "-n" ) == 0 )
		{
			if ( argv[2] != NULL )
				numProcs = atoi(argv[2]);
			else
				broken = 1;
			
			numberOfProcesses = numProcs;
			
			if ( argv[3] == NULL )
				broken = 1;
				
			if ( !broken && strcmp(argv[3], "-a" ) == 0 )
			{
				int resourceNum;
				int count = 4;
				int i;
				int j;
				for ( i = 0;; i++ )
				{
					if ( argv[count + i] == NULL )
						break;
						
					resources[i] = atoi( argv[count + i] );
					
					// Reallocate for another spot in the resources array
					temp = realloc( resources, ( i + 2 ) * sizeof( int ) );
					
					
					// If we are granted memory, keep going, else we're done
					if ( temp != NULL ) 
						resources = temp;
					else 
					{
						free(resources);
						exit(0);
					}
				}
				
				// Set the size of the array
				sizeOfArray = i;
				
				readyArray = malloc( numProcs * sizeof( int ));
				// Initialize the banker
				init_banker();
				set_available( resources );
				// At this point the program is done initializing
				
				printf("Processes: %d \n", numProcs);
				printf("Resources: \n");
				for( j = 0; j < i; j++ )
					printf("%d : %d \n",j, resources[j]);
				
				
				// Start the processes for the program ( threads )
				kick_off_processes( numProcs, resources );
				
				// Run the bankers algo
				run_bankers( numProcs, resources );
				
			}
			else
				input_error();
		
		}
		else
			input_error();
	}
	else
		input_error();
    
    free(resources);
    free(readyArray);
}