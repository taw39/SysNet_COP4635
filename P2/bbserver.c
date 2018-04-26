#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h> // for read()
#include <fcntl.h>

#define MAX_LEN 100000

/*
 * @author: Alexander Oldaker
 * @author: Tyler Webb
 * @date:   2018-04-14
 * @info:   COP4635 - Project 2
 */

int main(int argc, char **argv)
{
    int welcomeSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    int port = atoi(argv[1]);   // open this port for connections
    int numHosts = atoi(argv[2]);   // expected num of connections.

    welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

    // port set up
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero,'\0',sizeof(serverAddr.sin_zero));

    bind(welcomeSocket,(struct sockaddr *) &serverAddr, sizeof(serverAddr));

    if(listen(welcomeSocket,5)==0)
        printf("Server ready. Waiting for peer join request...\n");
    else
        printf("Error\n");

    addr_size = sizeof(serverStorage);

    // server while remain open until a force quit (ctrl + c)

    //make an array of port nums, send back a diff port num to each one.
    int i = 0;
    int newSocket[numHosts];
    char portArray[numHosts][50];



    while(i < numHosts){
        newSocket[i] = accept(welcomeSocket, (struct sockaddr *)&serverStorage,&addr_size);

        char rString[MAX_LEN];
        read(newSocket[i], rString, MAX_LEN);

        strtok(rString, ":");
        //portArray[i] = strtok(NULL, ":");
        strcpy(portArray[i], strtok(NULL, ":"));
        printf("Connected client with port %s!\n", portArray[i]);
        i++;
    }
    // should return to each peer the port number of their neighbor

    //assign token to the first port that connected
    for(i = 0; i < numHosts; i++){
        if(i == 0){
            strcat(portArray[i], ":1");
        }
        else{
            strcat(portArray[i], ":0");
        }
    }

    int k = 1;
    for(i = 0; i < numHosts; i++){
        //printf("Sending: %s\n", portArray[k]);
        write(newSocket[i], portArray[k], sizeof(portArray[k]));
        k++;
        if(k == numHosts)
            k = 0;
    }

    //close the program when all peers server expected have sent
    //connection messages and all have been informed of their neighbor.
    return 0;
}
