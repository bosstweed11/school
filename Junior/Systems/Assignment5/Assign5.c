//Assignment 5 Systems Software Andrew Beers 12/10/2012
//BUG the first run will append a '?' to the filename if you retype the same request it will process correctly


#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#define MYPORT 50005

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //mutex

//worker thread to process requests
void * do_stuff(void * p){
	int connfd = *(int *)p;
	char *buffer;
	char buf[1024];
	char rcv_buffer[1024];
	char *filename;
	char *file_data;
	
	char get_ref [4];
	char test_char[1];
	FILE *f;
	FILE *s;
	time_t rawtime;
  	struct tm * ptm;
  	int sz;
  	int first=1;
	int amt=0;
  	int i;
	int counter=0;
	
	get_ref[0] = 'G';
	get_ref[1] = 'E';
	get_ref[2] = 'T';
	get_ref[3] = ' ';
	
	
	
	//if you receive something, process it
	if(recv(connfd,rcv_buffer,1024,0)>0){
		
		//compare initial 4 chars to "GET "
		if(strncmp(rcv_buffer,get_ref,4)==0){
			int a = strlen(rcv_buffer);
			
			for(i=0;i<a;i++){
				printf("%c",rcv_buffer[i]);
			
			}
			printf("\n");
			//check that the string was long enough for both GET and HTTP/1.1
			if(a<13){
			
				
				strcpy(buf,"Use the format GET filename HTTP/1.1\n");
				//NO ROOM FOR GET HTTP/1.1
				//send info to the client
				while(amt < strlen(buf)) {
					int ret = send(connfd, buf+amt, strlen(buf)-amt, 0); 
	
					if(ret < 0) {
						perror("Send failed");
						exit(EXIT_FAILURE); 
					}
					amt += ret; 
				}
				
			}
			else{
				if(rcv_buffer[a-11] == ' ' &&rcv_buffer[a-10] == 'H' &&rcv_buffer[a-9] == 'T' &&rcv_buffer[a-8] == 'T'&&rcv_buffer[a-7] == 'P'&&rcv_buffer[a-6] == '/'&&rcv_buffer[a-5] == '1'&&rcv_buffer[a-4] == '.'&&rcv_buffer[a-3] == '1' ){
					
					//compare end of string to HTTP/1.1
					
					filename = malloc((a-15)*sizeof(char));
					// grab file name between GET and HTTP/1.1
					for(i=0;i<(a-15);i++){
						filename[i] = rcv_buffer[i+4];
					}
					
					
					// open the file from command line argument
					f= fopen(filename,"r+");
					// if the file exists proceed else 404 error
					if(f==NULL){
						amt=0;
						strcpy(buf,"HTTP/1.1 404 FILE NOT FOUND.\n\n");
						while(amt < strlen(buf)) {
							int ret = send(connfd, buf+amt, strlen(buf)-amt, 0); 
				
							if(ret < 0) {
								perror("Send failed");
								exit(EXIT_FAILURE); 
							}
							amt += ret; 
						}
					}
					else{
						fseek(f, 0, SEEK_END);
						sz = ftell(f);			//grab size of file
						fseek(f,0,SEEK_SET);
						
						file_data = malloc(sz*sizeof(char));	//malloc array for data
						
						strncpy(file_data,"",1);				//set data to 0
						while(!feof(f)){							//while there are characters to be read
							//read in first char
							fread(test_char,1,1,f);
							strncat(file_data,test_char,1);
							counter++;
							
							
						}
						counter--;
						strcpy(file_data,file_data); //append null
						
						
						time (&rawtime);
													//grab time
						ptm = gmtime (&rawtime);
					
					
						//print out formated response
						strcpy(buf,"HTTP/1.1 200 OK\n");
						while(amt < strlen(buf)) {
							int ret = send(connfd, buf+amt, strlen(buf)-amt, 0); 
	
							if(ret < 0) {
								perror("Send failed");
								exit(EXIT_FAILURE); 
							}
							amt += ret; 
						}
						memset(buf,0,1024);
						amt =0;
						
						//time
						sprintf(buf,"Date: %s", asctime(ptm));
						while(amt < strlen(buf)) {
							int ret = send(connfd, buf+amt, strlen(buf)-amt, 0); 
	
							if(ret < 0) {
								perror("Send failed");
								exit(EXIT_FAILURE); 
							}
							amt += ret; 
						}
						amt =0;
						memset(buf,0,1024);
						
						//size
						sprintf(buf,"Content-Length: %d\n",sz);
						while(amt < strlen(buf)) {
							int ret = send(connfd, buf+amt, strlen(buf)-amt, 0); 
	
							if(ret < 0) {
								perror("Send failed");
								exit(EXIT_FAILURE); 
							}
							amt += ret; 
						}
						memset(buf,0,1024);
						
						amt=0;
						strcpy(buf,"Connection: close\n");
						
						while(amt < strlen(buf)) {
							int ret = send(connfd, buf+amt, strlen(buf)-amt, 0); 
	
							if(ret < 0) {
								perror("Send failed");
								exit(EXIT_FAILURE); 
							}
							amt += ret; 
						}
						memset(buf,0,1024);
						amt =0;
						strcpy(buf,"Content-type: text/html\n\n");
						
						while(amt < strlen(buf)) {
							int ret = send(connfd, buf+amt, strlen(buf)-amt, 0); 
	
							if(ret < 0) {
								perror("Send failed");
								exit(EXIT_FAILURE); 
							}
							amt += ret; 
						}
						amt =0;
						buffer = malloc(sz*sizeof(char));
						
					
						sprintf(buffer,"%s",file_data);
						//send data
						while(amt < strlen(buffer)) {
						
							int ret = send(connfd, buffer+amt, strlen(buffer)-amt, 0); 
							
							if(ret < 0) {
								perror("Send failed");
								exit(EXIT_FAILURE); 
							}
							
							amt += ret; 
						}
						
						free(buffer);
						free(file_data);
					}
					
					
					counter =0;
					//close file
					fclose(f);		//close the file
					
				}
				else{
					amt=0;
					strcpy(buf,"Use the format GET filename HTTP/1.1\n");
					
					//no http/1.1
					while(amt < strlen(buf)) {
						int ret = send(connfd, buf+amt, strlen(buf)-amt, 0); 
						if(ret < 0) {
							perror("Send failed");
							exit(EXIT_FAILURE); 
						}
						amt += ret; 
					}
				}
			
			}
		}
		else{
			amt=0;
			strcpy(buf,"Use the format GET filename HTTP/1.1\n");
			//not GET
			while(amt < strlen(buf)) {
				int ret = send(connfd, buf+amt, strlen(buf)-amt, 0); 
				
				if(ret < 0) {
					perror("Send failed");
					exit(EXIT_FAILURE); 
				}
				amt += ret; 
			}
		}
	}
	
	//print client query to the file
	
	pthread_mutex_lock(&mutex);//lock so other threads cant simultaneously access the file
	s =fopen("stats.txt","a+");//open file
	fprintf(s,"%s",rcv_buffer);//print input
	fclose(s);	//close file
	pthread_mutex_unlock(&mutex);//unlock so the file can be accessed
	
	
	close(connfd); 
	
	return 0;
}

//main function
int main() {
	int sfd , connfd , amt = 0; 
	struct sockaddr_in addr; 
	pthread_t thread;
	
	//set up server
	if((sfd=socket(PF_INET , SOCK_STREAM ,0)) < 0) { 
		perror("Socket failed");
		exit(EXIT_FAILURE);
	}
	memset(&addr, 0, sizeof(addr)); 
	addr.sin_family = AF_INET; 
	addr.sin_port = htons(MYPORT); 
	addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(sfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) { 
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}
	if(listen(sfd, 10) < 0) {
		perror("Listen failed");
		exit(EXIT_FAILURE); 
	}
	
	//wait for connections
	while(1){
		if((connfd=accept(sfd, NULL, NULL)) < 0) { 
			perror("Accept failed"); 
			exit(EXIT_FAILURE);
		}
		pthread_create(&thread, NULL, do_stuff, (void *)&connfd); //spawn worker thread to process request
		
	}
	close(sfd); 
}
		






