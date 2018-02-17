#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include "p2p.h"
#include "list.h"

extern LIST* listRecv;
extern LIST* listSend;
extern int messageSize;

//Mutexes used for both recvList and sendList to allow for synchronization 
pthread_mutex_t sendMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t recvMutex = PTHREAD_MUTEX_INITIALIZER;

//Helper function to check if message to send contains any information i.e. has atleast one non-whitespace character
int is_empty(const char *str) {
  while (*str != '\0') {
    if (!isspace((unsigned char)*str))
      return 0;
    str++;
  }
  return 1;
}

void* keyboardInput(void* notUsed) { // void* parameter is required under p_thread
    while (1) {
        char msg[messageSize];
        fgets(msg, sizeof(msg), stdin);

        if (!is_empty(msg)) //If user inputed something i.e. atleast one non-whitespace char, add it to the send queue
        {   
            pthread_mutex_lock(&sendMutex);
            //Start of critical section i.e. listSend is a shared resource in memory
            ListPrepend(listSend,msg);
            //End of critical section
            pthread_mutex_unlock(&sendMutex);

            if (strlen(msg) == 2 && msg[0] == '!') {
                printf("\nClosing s-talk session...\n");
                sleep(1);
                exit(0);
            }

        }

    }
    pthread_exit(NULL);
}

void* sendMessage(void* p2pInfoPtr) {
    p2pClient p2pInfo = *(p2pClient*)p2pInfoPtr;
    while (1) {

        pthread_mutex_lock(&sendMutex);
        //Start of critical section i.e. listSend is a shared resource in memory
        if (ListCount(listSend) > 0) {
            char* msg = ListTrim(listSend);
            sendto(p2pInfo.sock, msg, messageSize, 0, (struct sockaddr*) &(p2pInfo.remoteClient), p2pInfo.addrLen);
        }
        //End of critical section
        pthread_mutex_unlock(&sendMutex);

    }
    pthread_exit(NULL);
}

void* recvMessage(void* p2pInfoPtr) {
    p2pClient p2pInfo = *(p2pClient*)p2pInfoPtr;
    while (1) {
        
        char msgRecv[messageSize];
        recvfrom(p2pInfo.sock, msgRecv, messageSize, 0, (struct sockaddr*) &(p2pInfo.remoteClient), &(p2pInfo.addrLen));

        pthread_mutex_lock(&recvMutex);
        //Start of critical section i.e. listRecv is a shared resource in memory
        ListPrepend(listRecv, msgRecv);
        //End of critical section
        pthread_mutex_unlock(&recvMutex);

    }
    pthread_exit(NULL);
}

void* printToScreen(void* p2pInfoPtr) {
    p2pClient p2pInfo = *(p2pClient*)p2pInfoPtr;
    while (1) {

        pthread_mutex_lock(&recvMutex);
        //Start of critical section i.e. listRecv is a shared resource in memory
        if (ListCount(listRecv) > 0) { 
            char* msg = ListTrim(listRecv);

            if (msg[0] == '!') {
                printf("\n%s has closed the s-talk session...\n", p2pInfo.remoteCompName);
                sleep(1);
                exit(0);
            }

            printf("%s: %s", p2pInfo.remoteCompName, msg);
            memset(msg,'\0', messageSize); // clear previous message
        }
        //End of critical section
        pthread_mutex_unlock(&recvMutex);

    }
    pthread_exit(NULL);
}