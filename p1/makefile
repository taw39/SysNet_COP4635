LIB	=	./lib
CFLAGS	=	-g -Wall
CC	=	gcc

clientObjects	=	client.o $(LIB)/clientLib.o
serverObjects	=	server.o $(LIB)/serverLib.o

httpClient	:	$(clientObjects)
	$(CC) -o httpClient $(clientObjects)

httpServer	:	$(serverObjects)
	$(CC) -o httpServer $(serverObjects)

client.o	:	httpClient.c $(LIB)/clientLib.h
server.o	:	httpServer.c $(LIB)/serverLib.h
$(LIB)/clientLib.o	:	$(LIB)/clientLib.c $(LIB)/clientLib.h
$(LIB)/serverLib.o	:	$(LIB)/serverLib.c $(LIB)/serverLib.h

.PHONY	:	clean
clean	:
	rm httpClient httpServer $(clientObjects) $(serverObjects)
