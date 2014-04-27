
# Specify the compiler
CC = g++

#CCOPTS = -ansi -pedantic -Wall -g
CCOPTS = -g -Wall
LIBS = -pthread

# Make the source
all:	arqSender arqReceiver router

common.o : common.h common.cpp 
	$(CC) $(CCOPTS) -c common.cpp
 
arqSender:  arqSender.cpp common.o  
	$(CC) $(CCOPTS) $(LIBS) common.o arqSender.cpp -o arqSender

arqReceiver: arqReceiver.cpp common.o
	$(CC) $(CCOPTS) $(LIBS) common.o arqReceiver.cpp -o arqReceiver

router: router.cpp common.o
	$(CC) $(CCOPTS) $(LIBS) common.o router.cpp -o router

clean :
	rm -f common.o arqSender arqReceiver router
