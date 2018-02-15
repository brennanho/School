typedef struct p2pClient {
    int sock;
    struct sockaddr_in remoteClient;
    char* remoteCompName;
    int sLen;
} p2pClient;

//Thread 1: Wait for keyboard input from client 
void* keyboardInput(void* nothing);

//Thread 2: When there are messages to print from the queue, print them
void* printToScreen(void* nothing);

//Thread 3: When there are messages to send from the queue, send them
void* sendMessage(void* p2pInfoPtr);

//Thread 4: When a message is received from the other client, append them to the queue
void* recvMessage(void* p2pInfoPtr);