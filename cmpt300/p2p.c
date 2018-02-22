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

//Shared resources generated in s-talk
extern LIST* listRecv;
extern LIST* listSend;
extern int messageSize;

//Mutexes and condition variables used for both recvList and sendList to enable synchronization 
pthread_mutex_t sendMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t recvMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t sendCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t recvCond = PTHREAD_COND_INITIALIZER;

//helper function to convert hostname to IP address
int hostname_to_ip(char * hostname, char* ip)
{
    struct hostent *he;
    struct in_addr **addr_list;

    he = gethostbyname(hostname);
    printf("%d",he);
    addr_list = (struct in_addr **) he->h_addr_list;
     
    if (addr_list[0] != NULL) // get the first IP address in the list
    {
        strcpy(ip , inet_ntoa(*addr_list[0]) );
        return 0;
    }
     
    return 1;
}

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
            
            if (ListCount(listSend) == nodesArrSize/2) // listSend gets half the capacity of available nodes
                pthread_cond_wait(&sendCond, &sendMutex);

            ListPrepend(listSend,msg);
            //End of critical section
            pthread_cond_signal(&sendCond); //Signals sendMessage to resume thread
            pthread_mutex_unlock(&sendMutex);

            if (strlen(msg) == 2 && msg[0] == '!') { //If local client enters in "!", the s-talk session will terminate
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

        if (ListCount(listSend) == 0)
            pthread_cond_wait(&sendCond, &sendMutex); //Blocks thread and waits for signal from recvMessage to consume item

        char* msg = ListTrim(listSend);
        //End of critical section
        pthread_cond_signal(&sendCond); //Signals keyboardInput to resume thread
        pthread_mutex_unlock(&sendMutex);

        sendto(p2pInfo.sock, msg, messageSize, 0, (struct sockaddr*) &(p2pInfo.remoteClient), p2pInfo.addrLen);

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

        if (ListCount(listRecv) == nodesArrSize/2) //listRecv gets half the capacity of available nodes
            pthread_cond_wait(&recvCond, &recvMutex);

        ListPrepend(listRecv, msgRecv);
        //End of critical section
        pthread_cond_signal(&recvCond); // Signals sendMessage to resume thread 
        pthread_mutex_unlock(&recvMutex);

    }
    pthread_exit(NULL);
}

void* printToScreen(void* p2pInfoPtr) {
    p2pClient p2pInfo = *(p2pClient*)p2pInfoPtr;
    while (1) {

        pthread_mutex_lock(&recvMutex);
        //Start of critical section i.e. listRecv is a shared resource in memory

        if (ListCount(listRecv) == 0) 
            pthread_cond_wait(&recvCond, &recvMutex); //Blocks thread and waits for signal from recvMessage to consume item

        char* msg = ListTrim(listRecv);
        //End of critical section
        pthread_cond_signal(&recvCond); // Signals recvMessage to resume thread
        pthread_mutex_unlock(&recvMutex); 

        if (msg[0] == '!') { // If remote client enters in "!", the s-talk session will close 
            printf("\n%s has closed the s-talk session...\n", p2pInfo.remoteCompName);
            close(p2pInfo.sock);
            sleep(1);
            exit(0);
        }

        printf("%s: %s", p2pInfo.remoteCompName, msg);
        memset(msg,'\0', messageSize); // clear previous message

    }
    pthread_exit(NULL);
}