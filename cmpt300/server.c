#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

//Server socket testing
int main(void) {

	int bufLen = 1024; 
	char buf[bufLen];
	struct sockaddr_in siLocal, siRemote;
	int sLen = sizeof(siRemote);


	siLocal.sin_family = AF_INET;
    siLocal.sin_port = htons(8888); // test port
    siLocal.sin_addr.s_addr = htonl(INADDR_ANY);

	int sock = socket(AF_INET, SOCK_DGRAM, 0); // why 0?

	bind(sock, (struct sockaddr*) &siLocal, sizeof(siLocal)); //what exactly is siLocal?

	while (1) {

		printf("Waiting for client data...\n");

		int recvLen = recvfrom(sock, buf, bufLen, 0, (struct sockaddr*) &siRemote, &sLen);

		printf("Client: %s\n", buf);

		sendto(sock, buf, recvLen, 0, (struct sockaddr*) &siRemote, sLen);


	}

	return 0;
}