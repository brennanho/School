typedef struct PCB {
	
	int id;
	int priority;
	int running;
	int burstTime;
	char* msg;

} PCB;

typedef struct Message {

	int fromID;
	int toID;
	char* msg;

} Message;

typedef struct Semaphore {

	int id;
	int val;

} Semaphore;

//create a process and put it on the appropriate ready Q
void Create(int priority);

//Copy the currently running processes and put it on the ready Q corresponding to the original process' priority 
//Attempting to Fork the "init" process shoudld fail
void Fork(void);

//Kill the named process and remove it from the system
void Kill(int pid);

//Kill the currently running process
void Exit(void);

//Time quantum of running process expires
void Quantum(void);

//Send a message to another process - block until reply
void Send(int pid, char* msg);

//Receive a message - block until one arrives
void Receive(void);

//Unblock sender and delivers a reply
void Reply(int pid, char* msg);

//Initialize the named semaphore with the value given
//ID's can take a value from 0 - 4. This can only be done once for a semaphore. Any subsequent attemps result in error
void NewSemaphore(int semID, int initVal);

//Execute the semaphore P operation on behalf of the running process. 
//You can assume semaphores IDs numbered 0 - 4
void SemaphoreP(int semID);

//Execute the semaphore V operation on behalf of the running process.
//You can assume semaphores IDs numbered 0 - 4
void SemaphoreV(int semID);

//Dump complete state information of process to screen (this includes process status etc)
void ProcessInfo(int pid);

//Display all process queues and their contents
void TotalInfo(void);