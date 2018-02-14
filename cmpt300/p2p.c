#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include "p2p.h"
#include "list.h"

extern LIST* listRecv;
extern LIST* listSend;
extern int messageSize;

//code from https://stackoverflow.com/questions/3981510/getline-check-if-line-is-whitespace, helper for keyboardInput
int is_empty(const char *s) {
  while (*s != '\0') {
    if (!isspace((unsigned char)*s))
      return 0;
    s++;
  }
  return 1;
}

void* keyboardInput(void* notUsed) { // void* parameter is required under p_thread
    while (1) {
        //fputs("You: ", stdout);
        char msg[messageSize];
        fgets(msg, sizeof(msg), stdin);
        if (!is_empty(msg)) //If user inputed something i.e. atleast one non-whitespace char, add it to the send queue
            ListPrepend(listSend,msg);
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

void* printToScreen(void* notUsed) { // void* parameter is required under p_thread
    while (1) {
        if (ListCount(listRecv) > 0) {
            char* msg = ListTrim(listRecv);
            printf("Friend: %s",msg);
            memset(msg,'\0', messageSize); // clear previous message
        }
    }
    return NULL;
}