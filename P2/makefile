CFLAGS	=	-g -Wall -std=gnu99 -pthread
CC	=	gcc

################################################################################

peerObjects	=	bbpeer.o peer_lib.o

bbPeer	:	$(peerObjects)
	$(CC) -o bbPeer $(peerObjects) $(CFLAGS)

bbpeer.o	:	bbpeer.c
peer_lib.o	:	peer_lib.c peer_lib.h

################################################################################

serverObjects	=	bbserver.o

bbserver.o	:	bbserver.c

bbServer	:	$(serverObjects)
	$(CC) -o bbServer $(serverObjects) $(CFLAGS)

################################################################################

.PHONY	:	clean all

clean	:
	rm bbPeer bbServer $(peerObjects) $(serverObjects)

all		:	bbPeer bbServer