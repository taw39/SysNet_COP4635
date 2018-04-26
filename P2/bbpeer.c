#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "peer_lib.h"
#include <pthread.h>
	// bbpeer localhost <portNum> <hostPort> <filenameBulletingBoard>
int main(int argc, char **argv) {
	if(argc < 4) { fprintf(stderr,"\tMissing arguments: Expecting 4\n"); exit(1); }

	char *address = "localhost"; // argv[1];   // Address of the server. Always localhost
	peer_port = atoi(argv[2]);   // Port you wish the peer to be on
	host_port = atoi(argv[3]);   // Port you wish to connect to
	file_path = argv[4];         // Path to file
	sequenceCount = 0;			 // The num of messages in our file.
	
	// init the space of the buffer. this is used by the user to prepare msg's
	buffer = (char*)malloc(sizeof(char) * MAX_BUFFER);

	// Holds response from server/peer we connect to
	char *response = malloc(sizeof(char) * CONV_LENGTH);
	strcpy(response,"\0");

	// Connect to server and save response
	connectServer(address, host_port, response);

	pthread_t tid_1;
	pthread_t tid_2;

	// Start a new startConnection thread

	pthread_create(&tid_1, NULL, logicRing, NULL);

	// Start a new user input thread
	
	pthread_create(&tid_2, NULL, userMenu, NULL);

	pthread_join(tid_1, NULL);
	pthread_join(tid_2, NULL);

	return 0;
}