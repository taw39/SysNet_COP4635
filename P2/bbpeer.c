#include <stdio.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h> // for read()
#include <fcntl.h>
// for the threads!
#include <pthread>

#define PORT        60001

#define MAX_BUFFER  100000

#define CONV_LENGTH 4096

// okay look, using globals was the easiest way to do this
// otherwise, we'd have to use something like this and I don't really one to
// https://stackoverflow.com/questions/1352749/multiple-arguments-to-function-called-by-pthread-create


/* The file pointer that stores the bulletin board */
FILE *fp;
/* The buffer the user will write to for the bb. When the token is recieved
    the thread will write the contents of the buffer to the shared file 
    before emtpying the buffer */
char *buffer;

int peerPort;

/**
 * Purpose:
 * A peer will access a shared file (the bulletin board) and read/write 
 * to it if it posses the token or not. The token is passed and recieved 
 * in one thread, and in another thread the user is inputing read/write
 * commands. 
 * 
 * A peer will connect to either a server first and then be added into 
 * a logical ring, or it will connect to a peer already in the logical ring
 * and will be added to it by that peer.
 * 
 * 
 * 
 * @author: Alexander Oldaker
 * @author: Tyler Webb
 * @date:   2018-04-14
 * @info:   COP4635 - Project 2
 */

// Refresher on pthreads: https://www.geeksforgeeks.org/multithreading-c-2/

/* write to the buffer */
void writeBuffer(){
    char *message = malloc(sizeof(char) * MAX_BUFFER);
    char *bodyBuffer = malloc(sizeof(char) * MAX_BUFFER);
    int messageNum = //get message number by counting the num of messages so far?
    strcpy("<message n=", message);
    strcpy(&messageNum, message);
    strcpy(">\n", message);

    printf("Please enter your message:\n");
    scanf("%s", bodyBuffer);
    strcpy(bodyBuffer, message);

    strcpy("</message>\n");

    strcpy(message, buffer);

}

/* Prints the user's menu */
void printMenu(){
    printf("\n********************************************\n");
    printf("-> Enter [w] for Write Operation 
        -- Appends a new message to the end of the message board\n");
    printf("-> Enter [r] for Read operation 
        -- Reads a particular message from the message board\n");
    printf("-> Enter [l] for List Operation 
        -- Display the range of sequence number of messages\n");
    printf("-> Enter [e] for Exit operation 
        -- Exits from the message board\n");
}

/**/
void *userMenu(){
    char choice;
    
    do{
        printMenu();
        printf("Please enter a choice:\n");
        scanf("%c", &choice);
        if(choice == 'w' || choice == 'W')
        {
            writeBuffer();
        }
        else if(choice == 'r' || choice == 'R')
        {

        }
        else if(choice == 'l' || choice == 'L')
        {

        }
        else if(choice == 'e' || choice == 'E')
        {

        }
        else
        {
            printf("Please enter a proper choice!\n")
        }
    }while(choice != 'e');

}

/* 
    this function runs the logic ring on this peer 
    
    it will constantly listen for messages that will id themselves as
        connection messages (new peer joining) or token messages (passing
        token over) in some sort of "header" on all messages
    
    the address of the neighbor it is passing the token to next needs to be
    passed into this function
*/
void *logicRing(void *vargp){
    printf("Starting logical ring!\n");
    int *neighborsPort = (int *)strtok(vargp, '\n');
    int *hasToken = (int *)strtok(NULL, \n);

    char *localhost = malloc(sizeof(char * 50));
    strcpy(localhost, "127.0.0.1");
    
    // set up a listening connection here for any incoming messages
    /*
    struct sockaddr_in remote_address;
    remote_address.sin_family = AF_INET;
    remote_address.sin_port = htons(*neighborsPort);
    inet_aton(localhost, &remote_address.sin_addr); //I assume all peers are on localhost?

    peer_socket = socket(AF_INET, SOCK_STREAM, 0);

    connect()
    */



    //if hasToken
        //write the global buffer to the file fp
    //else do nothing
    //loop back up
    //exit on some condition eventually
}

// Sends a "connect" message to the server/peer it 
//  which tells that target that this peer wants to be added to the ring.
void connectServer(char* address, int port, char *response){
    printf("Connecting to server...\n");
    
    //connect to server on the port given in host port.
    //create socket, bind it, and send/recieve from it

    /* start with creating to the remote address of the server */
    // copied over from project 1 for the most part
    struct sockaddr_in remote_address;
    remote_address.sin_family = AF_INET;
    remote_address.sin_port = htons(port);
    inet_aton(address, &remote_address.sin_addr);

    /* set up the port this peer will listen to */
    int peer_socket = socket(AF_INET, SOCK_STREAM, 0);

    /* 
        establish a connection between this peer and
        the server/peer it will connect to in order to join
        a logical ring
        
        This method is intended to connect to a server that establishes
        the logical ring and assigns the token ring AND ALSO connect 
        to a preexhisting logical ring and connect to the given 
        address of one of the members or it    
    */
    connect(peer_socket, (struct sockaddr *) &remote_address, sizeof(remote_address));

    /* set up the message to be sent to the server */
    // for now, this message will simply identify what kind of message it is
    // i.e. "CONNECT" for wanting to be added to the logical ring
    char *request;
    request = malloc(sizeof(char)*CONV_LENGTH);
    strcpy(request, "CONNECT");

    /* send the request message to the server */
    send(peer_socket, request, strlen(request), 0);
    /* wait for a response from the server and save it into response buffer */
    recv(peer_socket, &response, sizeof(response), 0);

    /*
        EXAMPLE RESPONSE FROM SERVER:

        60001\n
        0\n

        the first line is the port of the next peer to send the token to
        the second line is 0 = does not have token on start and 
        1 = has token on start

        (the lines are separated by a newline character!)
    */

   
    close(peer_socket);

    if(response[0] != '\0'){
        //server replied!
        printf("Connected to server!\n");
    }
    else
    {
        //server not reached!
        printf("Could not establish connection! Self-destructing!\n");
        exit(1);    // KA-BOOOOOM!
    }

    // exits cleanly, with the response buffer malloc'd in main holding
    // info on the next neighbor in the ring and if it has the token ring
    // at start

}

//bbpeer localhost <PortNum> <hostPort> <filenameBulletinBoard>
int main (int argc, char **argv){

    /* The address of the first connection made to the server */
    char* connectAddress = malloc(sizeof(char) * strlen(argv[1]));
    strcpy(connectAddress, argv[1]);    //always localhost though?
    /* The port number this peer will connect to on launch */
    int portNum = argv[2];
    peerPort = portNum; // just trust me on this one
    /* The port this peer (aka "host") will listen on */
    int hostPort = argv[3];
    /* Holds the response from the server/peer we connect to */
    /* Will contain the next neighbor and has token on start */
    char *response;
    response = malloc(sizeof(char) * CONV_LENGTH);
    strcpy(response, "\0");

    /* Connect to the server and save response back to response memory location */
    connectServer(connectAddress, hostPort, response);

    // start a new startConnection thread and feed in the hostPort
    // pthread_create(&tid, NULL, startConnection, argv[3]);

    // have a join for the above pthread. if it ends without exiting,
    // print the menu and assume the connection was made

    // thread ids for the twp threads we will need to use at the same time
    // pretty sure these are not meant to be assigned to by us
    int tid_1;
    int tid_2;

    

    /* start the thread that will listen for connection made*/
    pthread(&tid_1, NULL, logicRing, response);

    /* start the thread that will handle user input */
    // pthread_create(...);
    /* join with the logicRing thread before ending program */
    // pthread_join(tid_1, NULL);

    return 0;
}