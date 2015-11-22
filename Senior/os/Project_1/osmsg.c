//
//  main.c
//  OS_Project1
//
//  Created by Andy Beers on 5/27/14.
//  Copyright (c) 2014 Andy Beers. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>

struct message {
    uid_t user;   //This is “to” when sending and “from” when receiving
    char msg[140];
};

int main(int argc, const char * argv[])
{
    if ( argc  == 2 ) // read
    {
    	
    	// Get username from uid
    	struct passwd pwd_uid_to_name;
		struct passwd *result_uid;
		char *buf_uid;
		size_t bufsize_uid;
		int val_uid;
		bufsize_uid = 16384;
		buf_uid = malloc( bufsize_uid );
		
		if (buf_uid == NULL) 
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		

		//printf("Name: %s; UID: %ld0", pwd.pw_gecos, (long) pwd.pw_uid);
        struct message *getmsg = ( ( struct message *) malloc( sizeof( struct message) ) );

        // Get user id and put it in the message
		getmsg->user = geteuid();
		
        // call get message
      	long val3 = syscall( 325, getmsg );
      	
      	val_uid = getpwuid_r(getmsg->user, &pwd_uid_to_name, buf_uid, bufsize_uid, &result_uid);

		if (result_uid == NULL) 
		{
			if (val_uid == 0)
				printf("Not found\n");
			else 
				perror("getpwnam_r");
			exit(EXIT_FAILURE);
		}

        printf("%s said: \"%s\" ", pwd_uid_to_name.pw_name, getmsg->msg );
        
        if ( val3 == -1 )
        {
        	printf("Error\n");
        }
        else if ( val3 )
        {
        	printf("There are more messages for you!\n");
        }
        else
        {
        	printf("No more available messages\n");
        }
        
        free( getmsg);
        
    }
    else if ( argc == 4 ) // send
    {
    
    	// Get uid from user name
    	struct passwd pwd_name_to_uid;
		struct passwd *result_name;
		char *buf_name;
		size_t bufsize_name;
		int val_name; 
		bufsize_name = 16384;
		buf_name = malloc( bufsize_name );
		
		uid_t my_uid = geteuid();
		
		if (buf_name == NULL) 
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}
	
		// Get Password object to get uid
		val_name = getpwnam_r(argv[2], &pwd_name_to_uid, buf_name, bufsize_name, &result_name);
		
		if (result_name == NULL) 
		{
			if (val_name == 0)
				printf("Not found\n");
			else 
				perror("getpwnam_r");
			exit(EXIT_FAILURE);
		}


		// Create the message
        struct message *sendmsg;
        sendmsg = ( struct message *) malloc( sizeof( struct message) );
        
        // Put recipient uid in the message
        sendmsg->user = pwd_name_to_uid.pw_uid;
        
 		//Put text in message
        int length = sizeof(sendmsg->msg);
        strncpy( sendmsg->msg, argv[3], sizeof(sendmsg->msg)-1);
        
        
        // Call the send message syscall
        long val = syscall( 326, sendmsg );
        
        if ( val )
        {
        	printf("Message successfully sent.");
        }
        free( sendmsg);
        
    }
    else
    {
        printf("Please use either the read or write commands:\n osmsg -r\n osmsg -s <usrname> <msg>\n" );
    }
    
}



