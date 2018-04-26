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
#include <pthread.h>

#include "peer_lib.h"

void exitProgram(){
	// close the threads and exit(1)
}

void listFile(){
	// list the range of sequence numbers
	printf("The valid range of sequence numbers is: 0 - %d\n", sequenceCount);
	
}

void readFile(){
	// diplaying a specific message
	int num;
	// asking for what message to search for
	print("Please enter the sequence number of the message you would like to read: \n");
	scamf("%d", &num);
	
	// holds the file contents. hoping 1000 chars is enough space
	char *buff = malloc(sizeof(char) * 1000); // <-- Get rid of this magic number later
	
	FILE *fp = fopen(file_path, "r");
	fgets(buff, 1000, fp);

	// the string to look for in the buffer
	char *targetMessage = malloc(sizeof(char) * 20);
	sprintf(targetMessage, "<message n = %d>", num);

	// we will delimit the buffer until we find the target message
	char delim[10] = "</message>";	// <-- This will ensure we are only getting one message at a time.
	char *token;
	token = strtok(fp, delim);
	
	while( token != NULL){
		if(strstr(fp, targetMessage)){
			//found message. Print token.
			printf("Message found!\nPrinting Message...\n%s", token);
			break;
		}
	}
}

void writeBuffer() {
	char *body_buffer = malloc(sizeof(char) * MAX_BUFFER);
	fprintf(stdout,"Please enter your message:\n");
	scanf("%s",body_buffer);
	sequenceCount++;
	sprintf(buffer,"<message n=%d>\n%s</message>\n",sequenceCount,body_buffer);
}

void printMenu() {
	printf("\n********************************************\n");
	printf("-> Enter [w] for Write Operation -- Appends a new message to the end of the message board\n");
	printf("-> Enter [r] for Read operation -- Reads a particular message from the message board\n");
	printf("-> Enter [l] for List Operation -- Display the range of sequence number of messages\n");
	printf("-> Enter [e] for Exit operation -- Exits from the message board\n");
	printf("Please enter a choice:\n");
}

/**
 * UI layer
 */
void *userMenu() {
	char choice;

	do {
		printMenu();
		scanf("%c",&choice);

		switch(choice) {
			case 'w':
			case 'W':
				writeBuffer();
				break;
			case 'r':
			case 'R':
				readFile();
				break;
			case 'l':
			case 'L':
				listFile();
				break;
			case 'e':
			case 'E':
				//exitProgram();
				break;
			default:
				printf("Please enter a proper choice!\n");
				break;
		}
	} while(choice != 'e' || choice != 'E');

	printf("Exiting Program!\n");
}

/**
 * Communication layer
 */
void *logicRing(void *vargp) {
	fprintf(stderr,"Starting logical ring!\n");
	//int *neighbors_port = (int *)strtok(vargp, '\n');
	//int *has_token = (int *)strtok(NULL, '\n');

	// access value with '*n'
	int *n = (int *)vargp;	//<-- neighors port

	//if(debug) fprintf(stderr,"\n\tneighbors_port: %d\n\thas_token: %d", *neighbors_port, *has_token);

	int welcomeSocket, newSocket          ;
    struct sockaddr_in serverAddr         ;
    struct sockaddr_storage serverStorage ;
    socklen_t addr_size                   ;

	welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

	serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(*n);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero,'\0',sizeof(serverAddr.sin_zero));

    bind(welcomeSocket,(struct sockaddr *) &serverAddr, sizeof(serverAddr));

    if(listen(welcomeSocket,5)==0)
        printf("Peer ready. Listening for messages...\n");
    else
        printf("Error: Server not listening\n");	

	while(1){
		newSocket = accept(welcomeSocket, (struct sockaddr *)&serverStorage, &addr_size);

		char readString[MAX_BUFFER];
		read(newSocket, readString, MAX_BUFFER);

		/*
			Message types
			Token and send message count
			1. ex:
				"TOKEN:50"
			Connect and send this peer's port
			2. ex:
				"CONNECT:60001"
			Disconnect and send this peer's port
			3. ex:
				"DISCONNECT:60001"
		*/

		//call a function to determine the message type
		char *msgType = (char*)malloc(sizeof(char)*20);
		// get the msgType = TOKEN, CONNECT, or DISCONNECT
		strcpy(msgType, strtok(readString, ":"));
		if(strstr(msgType, "TOKEN") != NULL){
			// if token msg, get/set msg count, set hasToken to true (1) and write buffer.
			hasToken = 1;
			sequenceCount = atoi(strtok(NULL, ":"));
			//call void function that writes buffer to file 
			writeBuff();
			hasToken = 0;
			//call void function that sends a msg to next neighbor
			sendTokenMsg();
		}
		else if(strstr(msgType, "CONNECT")){
			//send this peer's neighbor to sender and use this recieved port as new neighbor
		}
		else if(strstr(msgType, "DISCONNECT")){
			// I don't know if we need to account for this.
		}
		else{
			printf("Error: Unrecognized message recieved in logic ring!\n");
		}
	}
}

void sendTokenMsg(){
	write(neighborPort, sequenceCount, sizeof(sequenceCount));
}

void writeBuff(){
	//open a filepath and write the buffer to it and close the file.

	fp = fopen(file_path, "w");

	fwrite(buffer , 1 , sizeof(buffer) , fp );

	fclose(fp);
}

void connectServer(char *address, int port, char *response) {
	fprintf(stderr,"Connecting to server...\n");

	/* set up the port this peer will listen to */
	int peer_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in remote_address;
	remote_address.sin_family = AF_INET;
	remote_address.sin_port = htons(port);
	inet_aton(address, &remote_address.sin_addr);

	connect(peer_socket, (struct sockaddr *) &remote_address, sizeof(remote_address));

	char *request;
	request = malloc(sizeof(char)*CONV_LENGTH);
	sprintf(request, "CONNECT:%d", peer_port);

	fprintf(stdout,"Waiting for server...\n");

	/* send the request message to the server */
	send(peer_socket, request, strlen(request), 0);

	/* wait for a response from the server and save it into response buffer */
	recv(peer_socket, &response, sizeof(response), 0);

	if(response[0] != '\0')
	{
		printf("Mission failed, we'll get em' next time. (Didn't get a neighbor port)\n");
		exit(1);
	}
	else
		fprintf(stdout, "Message recieved! Server port is: %s!\n", response);

	//response should be 
	//	60001:0 or 60001:1
	neighborPort = atoi(strtok(response, ":"));
	hasToken = atoi(strtok(NULL, ":"));
	// empty the response buffer just in case 
	strcpy(response, "\0");

	close(peer_socket);
}
