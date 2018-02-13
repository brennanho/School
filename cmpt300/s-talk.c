#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "p2p.h"
#include "list.h"

LIST* listRecv;
LIST* listSend;
size_t messageSize = 1024;

//helper function from "www.binarytides.com/hostname-to-ip-address-c-sockets-linux/""
int hostname_to_ip(char * hostname, char* ip)
{
    struct hostent *he;
    struct in_addr **addr_list;
    int i;

    he = gethostbyname(hostname);
    addr_list = (struct in_addr **) he->h_addr_list;
     
    for(i = 0; addr_list[i] != NULL; i++) 
    {
        strcpy(ip , inet_ntoa(*addr_list[i]) );
        return 0;
    }
     
    return 1;
}

//p2p UDP socket
int main(int argc, char *argv[]) {

    pthread_t sendThread, recvThread, printScreenThread, keyboardInputThread;
    struct sockaddr_in friendClient, myClient;
    int sLen = sizeof(myClient);

    listRecv = ListCreate();
    listSend = ListCreate();

    char hostname[1023] = "\0";
    char myIP[100] = "\0";
    gethostname(hostname, 1023);
    hostname_to_ip(hostname, myIP);

    myClient.sin_family = AF_INET;
    myClient.sin_port = htons(atoi(argv[1])); //my port
    myClient.sin_addr.s_addr = inet_addr(myIP);

    char friendIP[100] = "\0";
    hostname_to_ip(argv[2],friendIP);

    friendClient.sin_family = AF_INET;
    friendClient.sin_port = htons(atoi(argv[3])); //remote port (friend)
    friendClient.sin_addr.s_addr = inet_addr(friendIP);

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    p2pClient p2pInfo = {sock, friendClient, sLen};

    while (!bind(sock, (struct sockaddr *) &(myClient), sizeof(myClient)));

    printf("\nAttempting to connect to %s on port %s...\n", hostname, argv[3]);
    while (connect(sock, (struct sockaddr *) &(friendClient), sizeof(friendClient)) == -1); // keep attempting to connect to remote client
    printf("\nSuccessfully conneted to %s (IP: %s) on port %s\n\n", argv[2], friendIP, argv[3]);

    pthread_create(&sendThread, NULL, sendMessage, &p2pInfo);
    pthread_create(&recvThread, NULL, recvMessage, &p2pInfo);
    pthread_create(&printScreenThread, NULL, printToScreen, NULL);
    pthread_create(&keyboardInputThread, NULL, keyboardInput, NULL);

    pthread_join(sendThread, NULL);
    pthread_join(recvThread, NULL);
    pthread_join(printScreenThread, NULL);
    pthread_join(keyboardInputThread, NULL);


    return 0;
}