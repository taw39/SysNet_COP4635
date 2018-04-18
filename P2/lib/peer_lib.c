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

void writeBuffer() {
	char *body_buffer = malloc(sizeof(char) * MAX_BUFFER);

	fprintf(stdout,"Please enter your message:\n");
	scanf("%s",body_buffer);

	sprintf(buffer,"<message n=%d>\n%s</message>\n",message_count,body_buffer);
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
				break;
			case 'l':
			case 'L':
				break;
			case 'e':
			case 'E':
				break;
			default:
				printf("Please enter a proper choice!\n");
				break;
		}
	} while(choice != 'e' || choice != 'E');
}

/**
 * Communication layer
 */
void *logicRing(void *vargp) {
	fprintf(stderr,"Starting logical ring!\n");
	int *neighbors_port = (int *)strtok(vargp, '\n');
	int *has_token = (int *)strtok(NULL, '\n');

	if(debug) fprintf(stderr,"\n\tneighbors_port: %d\n\thas_token: %d", *neighbors_port, *has_token);

	char *localhost = "127.0.0.1"; // Hardcoded since project description doesn't clarify

	// Loop here
}

void connectServer(char *address, int port, char *response) {
	fprintf(stderr,"Connecting to server...\n");

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

	fprintf(stdout,"Waiting for server...");

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

	if(response[0] != '\0') {
		//server replied!
		fprintf(stdout,"Connected to server!\n");
	}
	else {
		//server not reached!
		fprintf(stderr,"Could not establish connection! Self-destructing!\n");
		exit(1);    // KA-BOOOOOM!
	}

	// exits cleanly, with the response buffer malloc'd in main holding
	// info on the next neighbor in the ring and if it has the token ring
	// at start
}
