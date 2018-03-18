#include <stdio.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h> // for read()
#include <fcntl.h>

#define PORT       60001    //has to be a valid port number (60001 - 60099)
#define MAX_LEN    1000000
#define HEADER_LEN 240

void response(int connSocket, char *fileName)
{
    struct stat filestat;
    char headerBuffer[HEADER_LEN];
    char fileBuffer[MAX_LEN];
    char filesize[7];
    FILE *fp;
    int fd;

    //the content type. i.e. html or jpg
    char *type = malloc(sizeof(char) * 100); 

    // get file stats for the file being requested
    fd = open(fileName, O_RDONLY);
    fstat(fd, &filestat);

    // track the file size
    sprintf(filesize,"%zd",filestat.st_size);

    fp = fopen(fileName,"r");

    if(fp == NULL)
    {
        // if no file exist, return a 404 message
        strcpy(headerBuffer,"HTTP/1.1 404 Not Found\r\nContent-Size: 203\r\n");
        fp = fopen("404.html","r");
    }

    else if(fp != NULL)
    {
        //if file does exist, determine the type and create header
        if((strstr(fileName, ".html")) != NULL){
            strcpy(type, "Content-Type: text/html \r\n");
        }else if((strstr(fileName, ".jpg")) != NULL){
            strcpy(type, "Content-Type: image/jpg \r\n");
        }
        
        strcpy(headerBuffer,"HTTP/1.1 200 OK\r\nContent-Length: ");
        strcat(headerBuffer,filesize);
        strcat(headerBuffer,"\r\n");
        strcat(headerBuffer, type);
        //DEBUG: Print the header
        //printf("Header:\n%s\n", headerBuffer);
    }

    strcat(headerBuffer,"Connection: keep-alive\r\n\r\n");

    // write the header to the socket
    write(connSocket,headerBuffer,strlen(headerBuffer));    
    fread(fileBuffer,sizeof(char),filestat.st_size + 1, fp); 
    fclose(fp);
    
    // write the file contents to the socket
    write(connSocket,fileBuffer,filestat.st_size);   

    close(connSocket);
}

int main()
{
    int welcomeSocket, newSocket          ;
    struct sockaddr_in serverAddr         ;
    struct sockaddr_storage serverStorage ;
    socklen_t addr_size                   ;

    welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

    // port set up
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero,'\0',sizeof(serverAddr.sin_zero));

    bind(welcomeSocket,(struct sockaddr *) &serverAddr, sizeof(serverAddr));

    if(listen(welcomeSocket,5)==0)
        printf("Server ready. Listening and waiting for client request\n");
    else
        printf("Error\n");

    addr_size = sizeof(serverStorage);
    
    // server while remain open until a force quit (ctrl + c)
    while(1){
        newSocket = accept(welcomeSocket, (struct sockaddr *)&serverStorage,&addr_size);

        char rString[MAX_LEN];
        read(newSocket, rString, MAX_LEN);

        // get just the name and extension of the file.
        char *fileName = (char*)malloc(sizeof(char)*MAX_LEN);
        strcpy(fileName,strtok(rString," "));
        strcpy(fileName,strtok(NULL   ," "));
        fileName++; // Gets rid of first '/'

        // build response to be sent to a socket given a specific file name.
        response(newSocket,fileName);
    }
    return 0;
}
