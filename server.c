#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>


#define NAME_SIZE 20

int main(int argc, char *argv[]) {
  

	int sockfd, newsockfd, portno, clilen, pid, game, bytes_recv;
	struct sockaddr_in serv_addr, cli_addr;
	char *client_name;
	char choice;
	
	client_name=(char*)malloc(NAME_SIZE*sizeof(char));
	
	game=0;
	
	if (argc < 2) {
        	printf("ERROR : INVALID USAGE\n");
		printf("(%s port \n",argv[0]);
        	return -1;
        }

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sockfd < 0){
		printf("ERROR opening socket");
		return -1;
	}


	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        	printf("ERROR on binding");
		return -1;
	}

	printf("RoosterNet Server\nDeveloped by PC\n");
	
	if(listen(sockfd,5)<0) {
		printf("ERROR : FAILED TO LISTEN\n");
		return -1;
	}

	clilen = sizeof(cli_addr);

	
	while (game=0) {
 		
		printf("Waiting for a chalenger...\n");
		
		newsockfd=accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
		
		if (newsockfd < 0){
     			printf("ERROR on accept");
			return -1;
		}
		
		memset(&client_name,0,sizeof(client_name)); // fill memory with constant bytes (0)
		
		bytes_recv=read(sock,client_name,NAME_SIZE);
		
		if(bytes_recv<0) {
			printf("ERROR : FAILED TO ACQUIRE DATA INFORMATION");
			return -1;
		}
	
		printf("New chalenger found : %s.\n",client_name);
		printf("Do you want to play with this challenger (Y/N)?)\n");
		scanf("%s",&choice);
		toupper(choice);
	
		if(strcmp(choice,"S");
			game=1;	
		
	}
	
	return 0;
}

/******** DOSTUFF() *********************
There is a separate instance of this function 
for each connection.  It handles all communication
once a connnection has been established.



void dostuff (int sock) {
	int n;
	char buffer[256];

	bzero(buffer,256);
	n = read(sock,buffer,255);
	if (n < 0) {
		printf("ERROR reading from socket");
		return -1;
	}
	
	printf("Here is the message: %s\n",buffer);
	n = write(sock,"I got your message",18);
	if (n < 0) {
		printf("ERROR writing to socket");
		return -1;
	}
}

*/

