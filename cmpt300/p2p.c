#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include "p2p.h"
#include "list.h"

extern LIST* listRecv;
extern LIST* listSend;
extern int messageSize;

void* keyboardInput(void* notUsed) { // void* parameter is required under p_thread
    while (1) {
        char msg[messageSize];
        fgets(msg, sizeof(msg), stdin);
        ListPrepend(listSend,msg);
    }
    return NULL;
}

void* printToScreen(void* notUsed) {
    while (1) {
        if (ListCount(listRecv) > 0) {
            char* msg = ListTrim(listRecv); // void* parameter is required under p_thread
            printf("Friend: %s",msg);
            memset(msg,'\0', messageSize); // clear previous message
        }
    }
    return NULL;
}

void* sendMessage(void* p2pInfoPtr) {
    p2pClient p2pInfo = *(p2pClient*)p2pInfoPtr;
    while (1) {
        if (ListCount(listSend) > 0) {
            char* msg = ListTrim(listSend);
            sendto(p2pInfo.sock, msg, messageSize, 0, (struct sockaddr*) &(p2pInfo.friendClient), p2pInfo.sLen);
        }
    }
    return NULL;
}

void* recvMessage(void* p2pInfoPtr) {
    p2pClient p2pInfo = *(p2pClient*)p2pInfoPtr;
    while (1) {
        char msgRecv[messageSize];
        recvfrom(p2pInfo.sock, msgRecv, messageSize, 0, (struct sockaddr*) &(p2pInfo.friendClient), &(p2pInfo.sLen));
        ListPrepend(listRecv, msgRecv);
    }
    return NULL;
}