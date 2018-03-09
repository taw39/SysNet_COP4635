// #include <stdio.h>
// #include <stdlib.h>
//
// #include "./lib/serverLib.h"
// /**
//  * While running sends file to request from httpClient / browser if it is a
//  * valid request. Otherwise will return an error message for displaying.
//  *
//  * @author: Tyler Webb
//  * @date:   2018-03-09
//  * @info:   COP4635 - Project 1
//  */
//
// int main(int argc,char **argv)
// {
//
//     return 0;
// }

/****************** SERVER CODE ****************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h> // for read()

int main(){
  int welcomeSocket, newSocket;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

  /*---- Create the socket. The three arguments are: ----*/
  /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;
  /* Set port number, using htons function to use proper byte order */
  serverAddr.sin_port = htons(7891);
  /* Set IP address to localhost */
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  /*---- Bind the address struct to the socket ----*/
  bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  /*---- Listen on the socket, with 5 max connection requests queued ----*/
  if(listen(welcomeSocket,5)==0)
    printf("Listening\n");
  else
    printf("Error\n");

  /*---- Accept call creates a new socket for the incoming connection ----*/
  addr_size = sizeof serverStorage;
  newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);

  // Parse out the recieved http response. 
  // read() returns number of bytes read while input contents of newSocket into rString for upto 4096 byes
  char rString[4096];
  int n = read(newSocket, rString, 4096);

  // DEBUG: prints the recieved message and size in bytes
  printf("Number of Bytes read: %d\nRecieved Request: %s\n", n, rString);

  // parse the file requested.
  char *fileName = (char *)malloc(sizeof(char) * 1024);
  char const a[2] = " ";
  strcpy(fileName, strtok(rString, a));
  strcpy(fileName, strtok(NULL, a));
  fileName += 1;  //this gets rid of the '\' in the front of the parsed file name.

  // DEBUG: prints the fileName to be returned
  printf("Requested File: %s\n", fileName);

  // open the file requested
  FILE *fp = fopen(fileName, "r");
  if(fp == NULL) {
      perror("Error opening file");
      return(-1);
   }
  fgets(buffer, 1024, fp);  // put the content of the requested file into the buffer

  /*---- Send message to the socket of the incoming connection ----*/
  //strcpy(buffer,"Hello World\n");
  send(newSocket,buffer,strlen(buffer),0);  // changed it so that it sends up to the length of buffer string.

  return 0;
}
