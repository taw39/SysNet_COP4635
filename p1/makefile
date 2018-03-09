LIB	=	./lib
CFLAGS	=	-g -Wall
CC	=	gcc

clientObjects	=	httpClient.o $(LIB)/clientLib.o
serverObjects	=	httpServer.o $(LIB)/serverLib.o

serverClient	:	$(clientObjects) $(serverObjects)
	$(CC) -o httpClient $(clientObjects); $(CC) -o httpServer $(serverObjects)

httpClient.o	:	httpClient.c
httpServer.o	:	httpServer.c
$(LIB)/clientLib.o	:	$(LIB)/clientLib.c $(LIB)/clientLib.h
$(LIB)/serverLib.o	:	$(LIB)/serverLib.c $(LIB)/serverLib.h

.PHONY	:	clean
clean	:
	rm httpClient httpServer $(clientObjects) $(serverObjects)
