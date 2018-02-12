#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>

typedef struct sendRecvInfo {
    int sock;
    char* msgSend;
    char* msgRecv;
    int msgLen;
    struct sockaddr_in server;
    int sLen;
} sendRecvInfo;

void* sendMessage(void* serverInfoptr) {
    sendRecvInfo serverInfo = *(sendRecvInfo*)serverInfoptr;
    while (!bind(serverInfo.sock, (struct sockaddr*) &(serverInfo.server), sizeof(serverInfo.server)));
    while (connect(serverInfo.sock, (struct sockaddr *) &(serverInfo.server), sizeof(serverInfo.server)) == -1); //attemptin to connect to remote machine
    while (1) {
        printf("myClient Input: ");
        fgets(serverInfo.msgSend, sizeof(serverInfo.msgSend), stdin);
        sendto(serverInfo.sock, serverInfo.msgSend, strlen(serverInfo.msgSend), 0, (struct sockaddr*) &(serverInfo.server), serverInfo.sLen);
    }
    return NULL;
}

void* recvMessage(void* serverInfoptr) {
    sendRecvInfo serverInfo = *(sendRecvInfo*)serverInfoptr;
    while (!bind(serverInfo.sock, (struct sockaddr*) &(serverInfo.server), sizeof(serverInfo.server)));
    while (connect(serverInfo.sock, (struct sockaddr *) &(serverInfo.server), sizeof(serverInfo.server)) == -1); //attemptin to connect to remote machine
    while (1) {
        memset(serverInfo.msgRecv,'\0', serverInfo.msgLen); // clear previous message
        recvfrom(serverInfo.sock, serverInfo.msgRecv, serverInfo.msgLen, 0, (struct sockaddr*) &(serverInfo.server), &(serverInfo.sLen));
        printf("Friend's reply: %s", serverInfo.msgRecv);
    }
    return NULL;
}

//myClient socket testing
int main(int argc, char *argv[]) {

    pthread_t sendThread, recvThread;
    int msgRecvLen = 1024; 
    char msgRecv[msgRecvLen];
    char msg[msgRecvLen];
    struct sockaddr_in myServer, friendServer, myClient;
    int sLen = sizeof(myServer);

    myServer.sin_family = AF_INET;
    myServer.sin_port = htons(8888); // test port
    myServer.sin_addr.s_addr = inet_addr("127.0.0.1");

    friendServer.sin_family = AF_INET;
    friendServer.sin_port = htons(8886); // test port
    friendServer.sin_addr.s_addr = inet_addr("127.0.0.1");

    myClient.sin_family = AF_INET;
    myClient.sin_port = htons(8889);
    myClient.sin_addr.s_addr = inet_addr("127.0.0.1");

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    int sock2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    sendRecvInfo serverInfoSend = {sock, msg, msgRecv, msgRecvLen, myServer, sLen};
    sendRecvInfo serverInfoRecv = {sock2, msg, msgRecv, msgRecvLen, friendServer, sLen};

    pthread_create(&sendThread, NULL, sendMessage, &serverInfoSend);
    pthread_create(&recvThread, NULL, recvMessage, &serverInfoRecv);

    pthread_join(sendThread, NULL);
    pthread_join(recvThread, NULL);


    return 0;
}