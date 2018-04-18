#include <stdio.h>
#include "lib/peer_lib.h"
#include <pthread.h>
	// bbpeer localhost <portNum> <hostPort> <filenameBulletingBoard>
int main(int argc, char **argv) {
	if(argc < 4) { fprintf(stderr,"\tMissing arguments: Expecting 4\n"); exit(1); }

	char *address = "localhost"; // argv[1];   // Address of the server. Always localhost
	peer_port = atoi(argv[2]);   // Port you wish the peer to be on
	host_port = atoi(argv[3]);   // Port you wish to connect to
	file_path = argv[4];         // Path to file

	// Holds response from server/peer we connect to
	char *response = malloc(sizeof(char) * CONV_LENGTH);
	strcpy(response,"\0");

	// Connect to server and save response
	connectServer(address, host_port, response);

	int tid_1;
	int tid_2;

	// Start a new startConnection thread and feed in the hostPort
	// pthread_create(&tid_1, NULL, startConnection, host_port);

	// Have a join for the above pthread. If it ends without exiting
	// print the menu and assume the connection was made
	pthread(&tid_1, NULL, logicRing, response);

	// Start the thread that will handle user input
	// pthread_create(...);
	// Join threads before ending program
	// pthread_join(tid_1, NULl);
	// pthread_join(tid_2, NULL);

	return 0;
}