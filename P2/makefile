CFLAGS	=	-g -Wall -lpthread
CC	=	gcc

peerObjects	=	bbPeer.o
serverObjects	=	bbServer.o

bbPeer	:	$(peerObjects) $(serverObjects)
	$(CC) -o bbPeer $(peerObjects); $(CC) -o bbServer $(serverObjects)

bbPeer.o	:	bbPeer.c
bbServer.o	:	bbServer.c

.PHONY	:	clean
clean	:
	rm httpClient httpServer $(peerObjects) $(serverObjects)

