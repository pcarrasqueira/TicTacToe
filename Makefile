CC=gcc
CFLAGS=-c -Wall
SOURCE1=server.c
SOURCE2=client.c
COMMON_SORCES=tictactoe.h
OBJECT1=$(SOURCE1:.c=.o)
OBJECT2=$(SOURCE2:.c=.o)
EXECUTABLE1=server
EXECUTABLE2=client


all: $(EXECUTABLE1) $(EXECUTABLE2) 

$(EXECUTABLE1): $(OBJECT1) 
	$(CC) $(OBJECT1) -o $(EXECUTABLE1)

$(EXECUTABLE2): $(OBJECT2) 
	$(CC) $(OBJECT2) -o $(EXECUTABLE2)


$(OBJECT1): $(SOURCE1)
	$(CC) $(CFLAGS) $(SOURCE1) 


$(OBJECT2): $(SOURCE2)
	$(CC) $(CFLAGS) $(SOURCE2) 

clean:
	\rm -f *.o
