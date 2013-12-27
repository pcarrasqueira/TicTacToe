#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char board[3][3];

void init() {

    int i, j;
    for (i=0;i<3;i++)
    	for (j=0;j<3;j++)
        	board[i][j]='-';
} 


void server_header() {
	printf("*********************************************\n");
        printf("*    RoosterNet Server Developed by PC      *\n");
	printf("*********************************************\n\n");
}


void client_header() {
	printf("*********************************************\n");
        printf("*    RoosterNet Client Developed by PC      *\n");
	printf("*********************************************\n\n");
}
void display_board() {
	printf("\n\n************* Tic-Tac-Toe Board *************\n\n");
	int i,j;
	for (i=0; i<3; i++) {
		printf("\t\t");
		for (j=0;j<3;j++)
			printf("%c  ",board[i][j]);
			printf("\n");
	}	

	printf("\n");
}


void move(int line,int col,char symbol) {

	int i,j;
	for (i=0; i<3; i++) {
                for (j=0;j<3;j++)
		if(i==line-1 && j==col-1)
			board[i][j]=symbol;
          }
  
}


int check_winner(char symbol) { // retur 1 if winner was found (change)

	if(board[0][0]==symbol && board[0][1]==symbol && board[0][2]==symbol)
		return 1;

	else if(board[1][0]==symbol && board[1][1]==symbol && board[1][2]==symbol)
                return 1;

	else if(board[2][0]==symbol && board[2][1]==symbol && board[2][2]==symbol)
                return 1;
	
	else if(board[0][0]==symbol && board[1][0]==symbol && board[2][0]==symbol)
                return 1;

	else if(board[0][1]==symbol && board[1][1]==symbol && board[2][1]==symbol)
                return 1;

	else if(board[0][2]==symbol && board[1][2]==symbol && board[2][2]==symbol)
                return 1;
	
	else if(board[0][0]==symbol && board[1][1]==symbol && board[2][2]==symbol)
                return 1;

	else if(board[0][2]==symbol && board[1][1]==symbol && board[2][0]==symbol)
                return 1;

	else
		return 0;
}


int check_move(int line,int col) { // return 1 if move is legal

	if(line >3 || col > 3 || line <1 || col < 1) {
		printf("Ilegal move, please play again.\n");
                return 0;
	}

	int i,j;
        for (i=0; i<3; i++) {
                for (j=0;j<3;j++) {
               		if(i==line-1 && j==col-1) {
                        	if(board[i][j]=='-')
					return 1;
				else{
					printf("Ilegal move, please play again.\n");
					return 0;
				}
			}
          	}	
	}
	return -1;
}
