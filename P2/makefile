LIB	=	./lib
CFLAGS	=	-g -Wall -lpthread
CC	=	gcc

peerObjects	=	bbPeer.o peer_lib.o
serverObjects	=	bbServer.o

bbPeer	:	$(peerObjects) $(serverObjects)
	$(CC) -o bbPeer $(peerObjects); $(CC) -o bbServer $(serverObjects)

bbPeer.o	:	bbPeer.c $(LIB)/peer_lib.h
peer_lib.o	:	$(LIB)/peer_lib.c $(LIB)/peer_lib.h
bbServer.o	:	bbServer.c

.PHONY	:	clean
clean	:
	rm httpClient httpServer $(peerObjects) $(serverObjects)

