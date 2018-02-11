#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>

//http://www.binarytides.com/hostname-to-ip-address-c-sockets-linux/
int hostname_to_ip(char * hostname , char* ip)
{
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
         
    if ( (he = gethostbyname( hostname ) ) == NULL) 
    {
        // get the host info
        herror("gethostbyname");
        return 1;
    }
 
    addr_list = (struct in_addr **) he->h_addr_list;
     
    for(i = 0; addr_list[i] != NULL; i++) 
    {
        //Return the first one;
        strcpy(ip , inet_ntoa(*addr_list[i]) );
        return 0;
    }
     
    return 1;
}

//Client socket testing
int main(int argc, char *argv[]) {

	int bufLen = 1024; 
	char buf[bufLen];
	char msg[bufLen];
    struct sockaddr_in server; // Server info?
	int sLen = sizeof(server);

	server.sin_family = AF_INET;
    server.sin_port = htons(8888); // test port
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	while (connect(sock, (struct sockaddr *) &server, sizeof(server)) == -1); //attemptin to connect to remote machine

	printf("Succesfully connected to remote socket\n");

	while (1) {

		printf("Client Input: ");
		fgets(msg, sizeof(msg), stdin);

		sendto(sock, msg, strlen(msg), 0, (struct sockaddr*) &server, sLen);

		recvfrom(sock, buf, bufLen, 0, (struct sockaddr*) &server, &sLen);

		printf("Server Reply: %s", buf);


	}

	return 0;
}