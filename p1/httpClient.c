#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define CONV_LENGTH 4096
/**
 * Requests the user for hame of the http server + name of a file that resides
 * on server. Sends HTTP/1.1 request to the server ofr hte file. This will
 * display all responses received from the httpServer. Connection will be
 * persistent. If the server has stopped working, this will be indicated.
 *
 * @author: Alexander Oldaker
 * @author: Tyler Webb
 * @date:   2018-03-09
 * @info:   COP4635 - Project 1
 */

int main (int argc, char **argv){
    char *addressFull = malloc(strlen(argv[1]) + 1);
    addressFull = argv[1];

    char *address = malloc((strstr(addressFull,":")) - addressFull + 1);
    address = strtok(addressFull,":");

    int port;
    addressFull = strtok(NULL,":");
    port = atoi(addressFull);

    char *filePath = malloc(strlen(argv[2]) + 1);
    filePath = argv[2];

    char *request = malloc(sizeof(char)*CONV_LENGTH);
    snprintf(request,sizeof(char)*CONV_LENGTH,"GET /%s HTTP/1.1\r\nHost:%s:%d\r\n\r\n",filePath,address,port);

    int client_socket;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    //connect to an address
    struct sockaddr_in remote_address;
    remote_address.sin_family = AF_INET;
    remote_address.sin_port = htons(port);
    inet_aton(address, &remote_address.sin_addr);

    connect(client_socket, (struct sockaddr *) &remote_address, sizeof(remote_address));

    char response[CONV_LENGTH];

    // DEBUG: prints the contents and size of request in bytes
    printf("Sent Request: %s\nLength of request %d\n", request, (int)strlen(request));

    send(client_socket, request, strlen(request), 0);       // originally sizeof(request) was only return 8, so only 8 bytes were sent
    recv(client_socket, &response, sizeof(response), 0);

    printf("Response from the server: %s\n", response);

    close(client_socket);
    return 0;
}
