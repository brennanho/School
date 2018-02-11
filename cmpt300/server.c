#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//server socket testing
int main(void) {

	int bufLen = 1024; 
	char buf[bufLen];
	struct sockaddr_in server, meClient, friendClient;
	int sLen = sizeof(meClient);

	server.sin_family = AF_INET;
    server.sin_port = htons(8888); // test port
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	while (!bind(sock, (struct sockaddr*) &server, sizeof(server))); //Attempting to bind socket...

	printf("Successfully binded socket\n");

	while (1) {

		int recvLen = recvfrom(sock, buf, bufLen, 0, (struct sockaddr*) &friendClient, &sLen);

		printf("Client: %s", buf);

		sendto(sock, buf, recvLen, 0, (struct sockaddr*) &meClient, sLen);

	}

	return 0;
}