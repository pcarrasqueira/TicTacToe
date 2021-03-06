#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdlib.h>
#include <unistd.h>

#include "functions.h"

#define SIZE_BUFFER 5
#define NAME_SIZE 15

int main (int argc,char *argv[] ) {

	int sockfd, port_nu,n,count,bytes_sent,bytes_rcv,line,col,flag,choice_symb;

	struct sockaddr_in serv_addr; //  basic structures for all syscalls and functions that deal with internet addresses
	struct hostent *server; // represent an entry in the hosts database
	char *name;
	char symbol,serv_symbol;
	char server_ip[16];
	char buffer[SIZE_BUFFER];
	
	
	name=(char*)malloc(NAME_SIZE*sizeof(char));

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

	system("clear");
	client_header();
    	printf("Enter your Name : ");

    	scanf("%s",name);
    
	n=write(sockfd,name,NAME_SIZE);
    	
	if(n<0) {  
        	printf("ERROR writing to socket");
		return -1;
	}

	inet_ntop(AF_INET,&serv_addr.sin_addr,server_ip,sizeof(serv_addr));
	printf("Address of a defender : %s\n",server_ip);

	n=read(sockfd,buffer,SIZE_BUFFER);
	
    	if(n<0) {
        	printf("ERROR reading from socket");
    		return -1;
	}

	if(strcmp(buffer,"00")==0) {
		printf("That defender denied you’re challenge.\n");
		return -1;
	}
	
	else if(strcmp(buffer,"01")==0) {
		printf("That defender accepted you’re challenge.\n");
		sleep(2);
		system("clear");
	}
	
	client_header();
	printf("Doing a sortition\n");
	
	bytes_rcv=read(sockfd,buffer,SIZE_BUFFER);

	if(bytes_rcv<0) {
		perror("ERROR : FAILED TO ACQUIRE DATA INFORMATION");
		return -1; 
	}

	if(strcmp(buffer,"00")==0) {
		printf("Defender plays first...\n");
		
		bytes_rcv=read(sockfd,buffer,SIZE_BUFFER);

		if(bytes_rcv<0) {
			perror("ERROR : FAILED TO ACQUIRE DATA INFORMATION");
			return -1; 
		}	
		
		if(strcmp(buffer,"01")==0) {
			symbol='O';
			serv_symbol='X';
			}
		else {
			symbol='X';
			serv_symbol='O';
		}
		flag=1;
		printf("You play with %c.\n",symbol);
		sleep(2);
	}else {
	
		printf("You play first\n");
		
		do{		
			printf("Choice symbol (1-X | 2-O) : ");
			scanf("%d",&choice_symb);
			if(choice_symb!=1 & choice_symb!=2)
				printf("Ilegal option, please choice again.\n");
		}while(choice_symb!=1 & choice_symb!=2);
		
		if(choice_symb==1) {
			symbol='X';
			serv_symbol='O';
			strcpy(buffer,"01");
			bytes_sent=write(sockfd,buffer,SIZE_BUFFER);	
		} else {
			symbol='O';
			serv_symbol='X';
			strcpy(buffer,"02");
			bytes_sent=write(sockfd,buffer,SIZE_BUFFER);	
		}

		if(bytes_sent<0) {
                        perror("ERROR : FAILED TO SEND DATA INFORMATION");
                        return -1;
              	 }
	
		flag=0;
	}

	count=0;
	system("clear");
	init();
	client_header();
	display_board();
	
	while(count<9) {

		if(flag==1) {

			bzero(buffer,SIZE_BUFFER);

			printf("The defender’s move...\n");

			bytes_rcv=read(sockfd,buffer,SIZE_BUFFER);

			if(bytes_rcv<0) {
				perror("ERROR : FAILED TO ACQUIRE DATA INFORMATION");
				return -1; 
			}

			line=buffer[0]-'0';
			col=buffer[1]-'0';
		
			move(line,col,serv_symbol);
			system("clear");
			client_header();
			display_board();
			count++;
		}
		else {

			do {
				printf("Your move...\n");
				printf("Line (1 to 3) : ");
				scanf("%d",&line);
				printf("Column (1 to 3) : ");
				scanf("%d",&col);
			}while(check_move(line,col)==0);
			
			system("clear");
			move(line,col,symbol);
			client_header();
			display_board();
			bzero(buffer,SIZE_BUFFER);
			
			buffer[0]=line+'0';
			buffer[1]=col+'0';

			bytes_sent=write(sockfd,buffer,SIZE_BUFFER);

			if(bytes_sent<0) {
				perror("ERROR : FAILED TO SEND DATA INFORMATION");
				return -1;
			}

			bzero(buffer,SIZE_BUFFER);
			count++;
		}

		if(count>4) {
			
			bytes_rcv=read(sockfd,buffer,SIZE_BUFFER);

                        if(bytes_rcv<0) {
                                perror("ERROR : FAILED TO ACQUIRE DATA INFORMATION");
                                return -1;
                        }

			
			if(strcmp(buffer,"00")==0) {
				printf("You lost the game\n");
				break;
			}
			else if(strcmp(buffer,"01")==0) {
				printf("You  won the game\n");
				break;
			}
                }

		if(flag==1)
                        flag=0;
                else
                        flag=1;

        }

	bytes_rcv=read(sockfd,buffer,SIZE_BUFFER);

	if(bytes_rcv<0) {
		perror("ERROR : FAILED TO ACQUIRE DATA INFORMATION");
		return -1;
	}

	if(strcmp(buffer,"11")==0)
		printf("TThe game ended with a draw\n");


	close(sockfd);
        free(name);
    	return 0;
}
