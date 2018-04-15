#include <stdio.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h> // for read()
#include <fcntl.h>

/*
 * @author: Alexander Oldaker
 * @author: Tyler Webb
 * @date:   2018-04-14
 * @info:   COP4635 - Project 2
 */

int main(int argc, char **argv)
{
    int welcomeSocket, newSocket          ;
    struct sockaddr_in serverAddr         ;
    struct sockaddr_storage serverStorage ;
    socklen_t addr_size                   ;

    int port = atoi(argv[1]);
    int numHosts = atoi(argv[2]);

    welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

    // port set up
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero,'\0',sizeof(serverAddr.sin_zero));

    bind(welcomeSocket,(struct sockaddr *) &serverAddr, sizeof(serverAddr));

    if(listen(welcomeSocket,5)==0)
        printf("Server ready. Listening and waiting for peer join request\n");
    else
        printf("Error\n");

    addr_size = sizeof(serverStorage);
    
    // server while remain open until a force quit (ctrl + c)

    while(1){
        newSocket = accept(welcomeSocket, (struct sockaddr *)&serverStorage,&addr_size);

        char rString[MAX_LEN];
        read(newSocket, rString, MAX_LEN);

        /*
            something something something
            this would be where we tell each peer that sends a connection
            message that tells them who is their neighbor pier.

            if its the first peer with the first connection message in the
            ring, give that peer the token first.
        */

       
    }

    //close done the program when all peers server expected have sent 
    //connection messages and all have been informed of their neighbor.
    return 0;
}
