LIB	=	./lib
CFLAGS	=	-g -Wall -lpthread
CC	=	gcc

################################################################################

peerObjects	=	bbpeer.o $(LIB)/peer_lib.o

bbPeer	:	$(peerObjects)
	$(CC) -o bbPeer $(peerObjects) $(CFLAGS)

bbpeer.o	:	bbpeer.c $(LIB)/peer_lib.h
$(LIB)/peer_lib.o	:	$(LIB)/peer_lib.c $(LIB)/peer_lib.h

################################################################################

serverObjects	=	bbserver.o

bbserver.o	:	bbserver.c

bbServer	:	$(serverObjects)
	$(CC) -o bbServer $($serverObjects) $(CFLAGS)

################################################################################

.PHONY	:	clean

clean	:
	rm bbPeer bbServer $(peerObjects) $(serverObjects)