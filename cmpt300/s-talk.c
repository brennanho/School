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
    struct sockaddr_in remoteClient, myClient;
    int sLen = sizeof(myClient);

    char myPort[1024];
    char remoteComp[1024];
    char remotePort[1024];
    strcpy(myPort, argv[1]);
    strcpy(remoteComp, argv[2]);
    strcpy(remotePort, argv[3]);

    listRecv = ListCreate();
    listSend = ListCreate();

    myClient.sin_family = AF_INET;
    myClient.sin_port = htons(atoi(myPort)); 
    myClient.sin_addr.s_addr = INADDR_ANY; // use IP address of my machine

    char remoteIP[100] = "\0";
    hostname_to_ip(remoteComp,remoteIP);

    remoteClient.sin_family = AF_INET;
    remoteClient.sin_port = htons(atoi(remotePort)); 
    remoteClient.sin_addr.s_addr = inet_addr(remoteIP);

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    p2pClient p2pInfo = {sock, remoteClient, remoteComp, sLen};

    while (!bind(sock, (struct sockaddr *) &(myClient), sizeof(myClient))); //Allows for port number to persist through connection

    printf("\nConnecting to %s (IP: %s) on port %s...\n", remoteComp, remoteIP, remotePort);
    while (connect(sock, (struct sockaddr *) &(remoteClient), sizeof(remoteClient)) == -1); // keep attempting to connect to remote client
    printf("Conneted to %s (IP: %s) on port %s\n\n", remoteComp, remoteIP, remotePort);

    //Spawn the four threads required for A2
    pthread_create(&sendThread, NULL, sendMessage, &p2pInfo);
    pthread_create(&recvThread, NULL, recvMessage, &p2pInfo);
    pthread_create(&printScreenThread, NULL, printToScreen, &p2pInfo);
    pthread_create(&keyboardInputThread, NULL, keyboardInput, NULL);

    //Force main thread to wait for all background threads to finish
    pthread_join(sendThread, NULL);
    pthread_join(recvThread, NULL);
    pthread_join(printScreenThread, NULL);
    pthread_join(keyboardInputThread, NULL);

    close(sock);

    return 0;
}