#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>


int main(){
	struct sockaddr_in addr;
	int sfd = socket(PF_INET, SOCK_STREAM, 0);
	int bind_err=0;
	int list_err=0;
	if(sfd ==-1){	

		printf("socket error\n");
		return 1;
	}

	memset(addr.sin_zero, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(50005);
	addr.sin_addr.s_addr = INADDR_ANY;
	
	bind_err =bind(sfd, (struct sockaddr *)&addr, sizeof(addr));
	
	if(bind_err==-1){
		printf("binding error\n");
		return 1;
	
	}
	
	list_err = listen(sfd,10);
	
	if(list_err ==-1){
		printf("listen error\n");
		return 1;
	
	}
	
	int connfd = accept(sfd, NULL, NULL);
	
	if(connfd ==-1){
		printf("connection error\n");
		return 1;
	
	}
	else{
		int send_amt;
		char buffer[1024];
		while(1){
			
			
			strcpy(buffer, "Hello there!");
			send_amt=send(connfd,buffer,strlen(buffer),0);
			
	
	
	
		}
	}
	
	close(sfd);
	
	
	
	



}
