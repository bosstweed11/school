#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <stdio.h> 
#include <stdlib.h>
#define MYPORT 50005
int main() {
	int sfd , connfd , amt = 0; 
	struct sockaddr_in addr; 
	char buf [1024];
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
	
	if((connfd=accept(sfd, NULL, NULL)) < 0) { 
		perror("Accept failed"); 
		exit(EXIT_FAILURE);
	}
	
	while(1){

		strcpy(buf, "Hello there!\n"); 
		while(amt < strlen(buf)) {
			int ret = send(connfd, buf+amt, strlen(buf)-amt, 0); 
			if(ret < 0) {
				perror("Send failed");
				exit(EXIT_FAILURE); 
			}
			amt += ret; 
		}
	}
	close(connfd); 
	close(sfd); 
	return 0;
}
