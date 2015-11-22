//
//  process.c
//  OS_Project2
//
//  Created by Andy Beers on 5/27/14.
//  Copyright (c) 2014 Andy Beers. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>


void release_resources ( int pid, int resources[] );
int request_resources( int pid, int resources[] );
void set_max_resources( int pid, int resources[] );

int* get_max_resources( int pid );
int* get_curr_resources( int pid );
void set_done ( int a, int pid );
int get_done();

int get_array_size();
int get_thread_id();
int get_ready();
void set_ready( int a, int pid );

int *myMaxResources;
int *myCurrentResources;
int *myRequestedResources;
pthread_mutex_t lock;


void *take_resources( void *resources_void_ptr )
{
	
	int i = 0;
	int *avail = (int * )resources_void_ptr;
	
	if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return NULL;
    }

	// Get the size of the array so we can allocate for our arrays
	int size = get_array_size();
	
	myMaxResources = malloc( size * sizeof( int ) );
	myCurrentResources = malloc( size * sizeof ( int ) );
	myRequestedResources = malloc( size * sizeof ( int ) );
	
	// We dont want to be preempted here...
	pthread_mutex_lock(&lock);
	
	int pid = get_thread_id();
	
	pthread_mutex_unlock(&lock);
	// Back to code
	
	// Seed rng
	srand( time(NULL) * ( pid + 5 ) );
	
	// Print outs
	
	//printf("MaxResources\n");
	pthread_mutex_lock(&lock);
	for( i = 0; i < size; i++ )
	{
		myCurrentResources[i] = 0;
		myMaxResources[i] = rand() % avail[i];
		//printf("Resource %d: %d\n", i, ( myMaxResources[i] ) );
	}
	pthread_mutex_unlock(&lock);

	// set max resources?
	pthread_mutex_lock(&lock);
	
	set_max_resources( pid, myMaxResources );
	
	pthread_mutex_unlock(&lock);
	
	
	//printf("Starting process %d\n", pid);
	
	set_ready( 1, pid );
	while( !get_ready())
	{
		//wait for everyone, thanks for no barriers OS X
	}
	
	
	while(1)
	{
		// not sure if this is necessary...
		pthread_mutex_lock(&lock);
		
		// Since threads share the same address space, we'll need to 
		// load in the correct values everytime we want to use them
		// stored in the banker
		myMaxResources = get_max_resources( pid );
		myCurrentResources = get_curr_resources( pid );


		// This loop will randomly assign a number resources to request
		for ( i = 0; i < size; i++ )
		{
			int r = rand();
			if ( myMaxResources[i] == 0 || myMaxResources[i] == myCurrentResources[i] )
			{
				myRequestedResources[i] = 0;
			}
			else if ( ( myMaxResources[i] - myCurrentResources[i] ) == 1 )
			{
				myRequestedResources[i] = r % 2;
			}
			else
			{
				int num = ( r % ( myMaxResources[i] - myCurrentResources[i]) );
				myRequestedResources[i] = ( r % ( myMaxResources[i] - myCurrentResources[i]) );
			}
		}
		
		// Make the request for resources
		int returnVal = request_resources( pid, myRequestedResources );
		
		pthread_mutex_unlock(&lock);
		
		if ( returnVal == 0 )
		{	
			// We have a safe state!
		}
		else
		{
			printf("Return val: %d\n", returnVal);
			
			// release all your resources
			release_resources(pid, get_curr_resources(pid) );
			break;
		}
		
		sleep(4);
		
		// Current resources are potentially different after a request
		myCurrentResources = get_curr_resources( pid );
		
		pthread_mutex_lock(&lock);
		// This loop will randomly assign a number resources to release
		for ( i = 0; i < size; i++ )
		{
			int r = rand();
			if ( myCurrentResources[i] == 0 )
			{
				myRequestedResources[i] = 0;
			}
			else if ( myCurrentResources[i] == 1 )
			{
				myRequestedResources[i] = r % 2;
			}
			else
			{
				myRequestedResources[i] = ( r % myCurrentResources[i] );
			}
		}
		
		
		
		// Release them
		release_resources ( pid, myRequestedResources );
		
		pthread_mutex_unlock(&lock);
		
		
		sleep(4);
      	
      	// If we are the last thread running, free up and terminate
      	if ( get_done() )
      	{
      		free( myMaxResources );
			free( myCurrentResources );
			free( myRequestedResources );
      		break;
      	}
	}

	printf("finished with process: %d\n", pid );
	pthread_mutex_destroy(&lock);
	
	set_done(1,pid);
	
	
	
}

//setters getters for max array, avail array