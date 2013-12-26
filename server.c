#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "functions.h"

#define NAME_SIZE 15
#define SIZE_BUFFER 5

int main(int argc, char *argv[]) {
  

	int sockfd, newsockfd, portno, clilen, game, bytes_recv,bytes_sent,line,col,count, flag,aux,aux2;
	struct sockaddr_in serv_addr, cli_addr;
	char *client_name;
	char buffer[SIZE_BUFFER];
	char choice,symbol,client_symbol;
	
	
	symbol='X';
	client_symbol='O';
	
	game=0;

	client_name=(char*)malloc(NAME_SIZE*sizeof(char));

	
	if (argc < 2) {
        	printf("ERROR : INVALID USAGE\n");
		printf("(%s port \n",argv[0]);
        	return -1;
        }

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	
	if (sockfd < 0){
		perror("ERROR opening socket");
		return -1;
	}


	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
		perror("ERROR on binding\n");
		return -1;
	}

	
	if(listen(sockfd,5)<0) {
		perror("ERROR : FAILED TO LISTEN\n");
		return -1;
	}

	clilen = sizeof(cli_addr);

	system("clear");
	server_header();
	do {
 		bzero(client_name,NAME_SIZE);
		printf("Waiting for a chalenger...\n");
		
		newsockfd=accept(sockfd,(struct sockaddr *)&cli_addr,&clilen);
		
		if (newsockfd < 0){
     			perror("ERROR on accept");
			return -1;
		}
		
		
		bytes_recv=read(newsockfd,client_name,NAME_SIZE);
		
		if(bytes_recv<0) {
			perror("ERROR : FAILED TO ACQUIRE DATA INFORMATION");
			return -1;
		}
	
		printf("New chalenger found : %s\n",client_name);
		printf("Do you want to play with this challenger (Y/N)?)\n");
		scanf(" %c",&choice);
		
		bzero(buffer,SIZE_BUFFER);

		if(choice=='Y' || choice=='y') {
			strcpy(buffer,"01");
			bytes_sent=write(newsockfd,buffer,SIZE_BUFFER);
    			
                        if(bytes_sent<0) {
                        perror("ERROR : FAILED TO SEND DATA INFORMATION");
                        return -1; 
                        }
			bzero(buffer,SIZE_BUFFER);
			game=1;
			system("clear");
		}
		else if(choice=='N' || choice=='n') {
			strcpy(buffer,"00");

			bytes_sent=write(newsockfd,buffer,SIZE_BUFFER);
			
			if(bytes_sent<0) {
                        perror("ERROR : FAILED TO SEND DATA INFORMATION");
                        return -1;
              	 	}

			close(newsockfd);
			continue;
			}
	
		else {
			printf("Option not available\n");
			return -1;
			}
		
	} while(game==0);

	init();
	server_header();
	display_board();
	
	count=0;
	flag=0;
	aux=0;
	aux2=0;

	while (count<9) { 

		if(flag==0) {
	
			do{
			printf("Your move...\n");
			printf("Line (1 to 3) : ");
			scanf("%d",&line);
			printf("Column (1 to 3) : ");
			scanf("%d",&col);
			}while(check_move(line,col)==0);
			
			move(line,col,symbol);
			system("clear");
			server_header();
			display_board();

			buffer[0]=line+'0';
			buffer[1]=col+'0';
		
			bytes_sent=write(newsockfd,buffer,SIZE_BUFFER);

			if(bytes_sent<0) {
				perror("ERROR : FAILED TO SEND DATA INFORMATION");
				return -1;
			}

			bzero(buffer,SIZE_BUFFER);
			count++;
		} 
		else {
		
			printf("\nThe challenger’s move...\n");
			
			bytes_recv=read(newsockfd,buffer,SIZE_BUFFER);

			if(bytes_recv<0) {
				perror("ERROR : FAILED TO ACQUIRE DATA INFORMATION");
				return -1;
			}
			
			line=buffer[0]-'0';
			col=buffer[1]-'0';
			move(line,col,client_symbol);
			system("clear");
			server_header();
			display_board();
			count++;
		}

		
		
		if(count>4) { // min 5 moves to found a winner

			aux=check_winner(symbol);
			aux2=check_winner(client_symbol);
			
			if(aux==1) {
			
				printf("You won the game\n");

				strcpy(buffer,"00");
				bytes_sent=write(newsockfd,buffer,SIZE_BUFFER);
				
				if(bytes_sent<0) {
					perror("ERROR : FAILED TO SEND DATA INFORMATION");
					return -1; 
				}
				break;
			}
		
			else if(aux2==1) {
                 
                                printf("You lost the game\n");

                                strcpy(buffer,"01");
                                bytes_sent=write(newsockfd,buffer,SIZE_BUFFER);
                         
                                if(bytes_sent<0) {
                                        perror("ERROR : FAILED TO SEND DATA INFORMATION");
                                        return -1;
                                }
                                break;
                        }
			else {
				strcpy(buffer,"22");
                                bytes_sent=write(newsockfd,buffer,SIZE_BUFFER);

                                if(bytes_sent<0) {
                                        perror("ERROR : FAILED TO SEND DATA INFORMATION");
                                        return -1;
                                }
			}

				
		}
		
		if(flag==1)
			flag=0;
		else
			flag=1;
		
	}
	
	if(aux==0 && aux2==0) {
		
		printf("The game ends in a draw\n");
		
		strcpy(buffer,"11");
                
		bytes_sent=write(newsockfd,buffer,SIZE_BUFFER);
                
		if(bytes_sent<0) {
                	perror("ERROR : FAILED TO SEND DATA INFORMATION");
                        return -1;
               	}
	}
	
	
	strcpy(buffer,"22");
	bytes_sent=write(newsockfd,buffer,SIZE_BUFFER);
	if(bytes_sent<0) {
		perror("ERROR : FAILED TO SEND DATA INFORMATION");
		return -1;
	}

	close(sockfd);
	close(newsockfd);
	free(client_name);

	return 0;
}

