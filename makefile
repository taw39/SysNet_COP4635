CFLAGS	=	-g -Wall
CC	=	gcc

clientObjects	=	httpClient.o
serverObjects	=	httpServer.o

serverClient	:	$(clientObjects) $(serverObjects)
	$(CC) -o httpClient $(clientObjects); $(CC) -o httpServer $(serverObjects)

httpClient.o	:	httpClient.c
httpServer.o	:	httpServer.c

.PHONY	:	clean
clean	:
	rm httpClient httpServer $(clientObjects) $(serverObjects)
