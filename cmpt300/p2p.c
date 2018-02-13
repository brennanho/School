#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include "list.h"

typedef struct p2pClient {
    int sock;
    char* msgSend;
    char* msgRecv;
    int msgLen;
    struct sockaddr_in friendClient;
    int sLen;
} p2pClient;

void* keyboardInput() {
    while (1) {
        
    }
    return NULL;
}

void* printToScreen() {
    while (1) {
        
    }
    return NULL;
}

void* sendMessage(void* p2pInfoPtr) {
    p2pClient p2pInfo = *(p2pClient*)p2pInfoPtr;
    while (1) {
        fgets(p2pInfo.msgSend, sizeof(p2pInfo.msgSend), stdin);
        sendto(p2pInfo.sock, p2pInfo.msgSend, strlen(p2pInfo.msgSend), 0, (struct sockaddr*) &(p2pInfo.friendClient), p2pInfo.sLen);
    }
    return NULL;
}

void* recvMessage(void* p2pInfoPtr) {
    p2pClient p2pInfo = *(p2pClient*)p2pInfoPtr;
    while (1) {
        memset(p2pInfo.msgRecv,'\0', p2pInfo.msgLen); // clear previous message
        recvfrom(p2pInfo.sock, p2pInfo.msgRecv, p2pInfo.msgLen, 0, (struct sockaddr*) &(p2pInfo.friendClient), &(p2pInfo.sLen));
        printf("Friend: %s", p2pInfo.msgRecv);
    }
    return NULL;
}

LIST* listRecv;
LIST* listSend;

//p2p UDP socket
int main(int argc, char *argv[]) {

    pthread_t sendThread, recvThread;
    int msgRecvLen = 1024; 
    char msgRecv[msgRecvLen];
    char msg[msgRecvLen];
    struct sockaddr_in friendClient, myClient;
    int sLen = sizeof(myClient);

    listRecv = ListCreate();
    listSend = ListCreate();

    myClient.sin_family = AF_INET;
    myClient.sin_port = htons(atoi(argv[1])); //my port
    myClient.sin_addr.s_addr = inet_addr("127.0.0.1");

    friendClient.sin_family = AF_INET;
    friendClient.sin_port = htons(atoi(argv[2])); //remote port (friend)
    friendClient.sin_addr.s_addr = inet_addr("127.0.0.1");

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    p2pClient p2pInfo = {sock, msg, msgRecv, msgRecvLen, friendClient, sLen};

    while (!bind(sock, (struct sockaddr *) &(myClient), sizeof(myClient))); 
    while (connect(sock, (struct sockaddr *) &(friendClient), sizeof(friendClient)) == -1); // keep attempting to connect to remote client

    pthread_create(&sendThread, NULL, sendMessage, &p2pInfo);
    pthread_create(&recvThread, NULL, recvMessage, &p2pInfo);

    pthread_join(sendThread, NULL);
    pthread_join(recvThread, NULL);


    return 0;
}