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
#define MAX_LEN    4096
#define HEADER_LEN 240

// https://stackoverflow.com/questions/28631767/sending-images-over-http-to-browser-in-c

// https://stackoverflow.com/questions/25411046/send-image-through-c-socket
void response(int connSocket, char *fileName)
{
    struct stat filestat;
    char headerBuffer[HEADER_LEN];
    char fileBuffer[MAX_LEN];
    //char filesize[7];
    FILE *fp;
    int fd;

    //the content type. i.e. html or jpg
    char *type = malloc(sizeof(char) * 100); 

    if((fd = open(fileName, O_RDONLY)) < -1 || (fstat(fd,&filestat)<0))
        printf("Error in measuring the size of the file");

    //sprintf(filesize,"%zd",filestat.st_size);

    fp = fopen(fileName,"r");

    if(fp == NULL)
    {
        // Measured 404.html size beforehand
        strcpy(headerBuffer,"HTTP/1.1 404 Not Found\r\nContent-Size: 203\r\n");
        fp = fopen("404.html","r");
    }

    else if(fp != NULL)
    {
        // determines the content type but the browser should be able to determine that automatically.
        
        //determine file type
        if((strstr(fileName, ".html")) != NULL){
            strcpy(type, "text/html");
        }else if((strstr(fileName, ".jpg")) != NULL){
            strcpy(type, "image/jpg");
        }
        
        stat(fileName, &filestat);
        //strcpy(headerBuffer,"HTTP/1.1 200 OK\r\nContent-Length: ");
        //strcat(headerBuffer,filesize);
        //strcat(headerBuffer,"\r\n");
        sprintf(headerBuffer, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %lld\r\n", type, filestat.st_size);
        // contenet type?

    }

    strcat(headerBuffer,"Connection: keep-alive\r\n\r\n");
    write(connSocket,headerBuffer,strlen(headerBuffer));    
    fread(fileBuffer,sizeof(char),filestat.st_size, fp); // check if this is getting all of the image?
    fclose(fp);
    
    //write(connSocket,fileBuffer,filestat.st_size);     // MUST BE THE PROBLEM

    // gets the whole buffer sent
    char *msg = fileBuffer;
    int msglen = strlen(msg);
    while(msglen > 0){
        int len = write(connSocket, msg, msglen);
        if(len <=0){
            // do nothing -- will exit afterwards
        }else{
            msg += len;
            msglen -= len;
        }
    }
    
    close(connSocket);
}

int main()
{
    int welcomeSocket, newSocket          ;
    struct sockaddr_in serverAddr         ;
    struct sockaddr_storage serverStorage ;
    socklen_t addr_size                   ;

    welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

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
    
    while(1){
        newSocket = accept(welcomeSocket, (struct sockaddr *)&serverStorage,&addr_size);

        char rString[MAX_LEN];
        read(newSocket, rString, MAX_LEN);

        char *fileName = (char*)malloc(sizeof(char)*MAX_LEN);
        strcpy(fileName,strtok(rString," "));
        strcpy(fileName,strtok(NULL   ," "));
        fileName++; // Get rid of first '/'

        response(newSocket,fileName);
    }
    return 0;
}
