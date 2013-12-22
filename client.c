#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdlib.h>



int main (int argc,char *argv[] ) {

int sockfd, port_nu,n;

struct sockaddr_in serv_addr; //  basic structures for all syscalls and functions that deal with internet addresses
struct hostent *server; // represent an entry in the hosts database

char buffer[256];


	if(argc<3) {

		printf("ERROR : INVALID USAGE \n(%s host port )\n",argv[0]);
		return -1;
	}

	port_nu=atoi(argv[2]);
	sockfd=socket(AF_INET,SOCK_STREAM,0);

	if(sockfd<0) { /* Create the socket  */
		printf("ERROR CREATING SOCKET\n");
		return -1;
	}

	server = gethostbyname(argv[1]);
	if(server==NULL) {
		printf("ERROR : NO SUCH HOST\n");
		return -1;
	}
	
	bzero((char*)&serv_addr,sizeof(serv_addr)); //Initializaton by write zero-value  bites
	/* Give the socket a name.  */
	serv_addr.sin_family=AF_INET;
	bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
	serv_addr.sin_port = htons(port_nu);
	
	
	/* Client-Server comunication */

	if(connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0) { 
        	printf("ERROR connecting");
		return -1;
	}
	
    	printf("Please enter the message: ");
    	bzero(buffer,256);
    	fgets(buffer,255,stdin);
    	n=write(sockfd,buffer,strlen(buffer));
    	
	if(n<0) {  
        	printf("ERROR writing to socket");
		return -1;
	}
    	bzero(buffer,256);
    	n=read(sockfd,buffer,255);
	
    	if(n<0) {
        	printf("ERROR reading from socket");
    		return -1;
	}

	printf("%s\n",buffer);
    	return 0;

	






}
