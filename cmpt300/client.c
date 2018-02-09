#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Client socket testing
int main(int argc, char *argv[]) {

	int bufLen = 1024; 
	char buf[bufLen];
	char msg[bufLen];
	struct sockaddr_in siLocal, siRemote;
	int sLen = sizeof(siRemote);

	siLocal.sin_family = AF_INET;
    siLocal.sin_port = htons(8888); // test port

	int sock = socket(AF_INET, SOCK_DGRAM, 0); // why 0?

	inet_aton("127.0.0.1", &siRemote.sin_addr);

	while (1) {

		printf("Client Input: ");
		gets(msg);

		sendto(sock, msg, strlen(msg), 0, (struct sockaddr*) &siRemote, sLen);

		recvfrom(sock, buf, bufLen, 0, (struct sockaddr*) &siRemote, &sLen);

		puts(buf);


	}

	return 0;
}