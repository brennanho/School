#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "list.h"
#include "process.h"

LIST* readyQHIGH;
LIST* readyQMED;
LIST* readyQLOW;
LIST* blockedList;
LIST* messageList;
PCB* runningProc;
PCB* initProc;
int idCount = 0;
int quantum = 5;

void inputCommands(void) {
	
	while (1) {

		int cmdSize = 4;
		int paramSize = 4;
		int msgSize = 40;
		char cmd[cmdSize];
		char param[paramSize];
		char msg[msgSize];
		printf("\nCommand: ");
        fgets(cmd, cmdSize, stdin);
 
        
        if (strlen(cmd) == 2) {

	    	if (cmd[0] == 'c' || cmd[0] == 'C') {
	    		printf("Create - Please enter the priority of the new process: 0 (high), 1 (medium), 2 (low)\n");
				fgets(param, paramSize, stdin);

				if ((strlen(param) == 2) & (param[0] == '0' || param[0] == '1' || param[0] == '2')) {
		    		int priority = param[0] - '0';
		    		Create(priority);
		    	} else {
		    		printf("Invalid parameter\n");
		    	}

	    	} else if (cmd[0] == 'f' || cmd[0] == 'F') {
	    		printf("\nForking process (ID: %d)\n", runningProc->id);
	    		Fork();

	    	} else if (cmd[0] == 'k' || cmd[0] == 'K') {
	    		printf("Kill - To kill a process, enter its process id\n");
	    		fgets(param, paramSize, stdin);
	    		int id = atoi(param);
	    		Kill(id);


	    	} else if (cmd[0] == 'e' || cmd[0] == 'E') {
	    		printf("Exit - kills the current process and begins a new process\n");
	    		Exit();

	    	} else if (cmd[0] == 'q' || cmd[0] == 'Q') {
	    		printf("Quantum - CPU time expired\n");
	    		Quantum();

	    	} else if (cmd[0] == 's' || cmd[0] == 'S') {
	    		printf("Send - Please enter the ID of the process to send (blocking)\n");
	    		fgets(param, paramSize, stdin);
	    		int id = atoi(param);
	    		printf("Send - Please enter your message\n");
	    		fgets(msg, msgSize, stdin);
	    		Send(id,msg);

	    	} else if (cmd[0] == 'r' || cmd[0] == 'R') {
	    		printf("Receive: currenty process will block\n");
	    		Receive();

	    	} else if (cmd[0] == 'y' || cmd[0] == 'Y') {
	    		printf("Command: Reply\n");

	    	} else if (cmd[0] == 'n' || cmd[0] == 'N') {
	    		printf("Command: New Semaphore\n");

	    	} else if (cmd[0] == 'p' || cmd[0] == 'P') {
	    		printf("Command: Semaphore P\n");

	    	} else if (cmd[0] == 'v' || cmd[0] == 'V') {
	    		printf("Command: Semaphore V\n");

	    	} else if (cmd[0] == 'i' || cmd[0] == 'I') {
	    		printf("\nProcess Info - Please enter the ID of the process to retrieve info\n");
	    		fgets(param, paramSize, stdin);
	    		int id = atoi(param);
	    		ProcessInfo(id);

	    	} else if (cmd[0] == 't' || cmd[0] == 'T') {
	    		TotalInfo();

	    	} else {
	    		printf("Invalid command\n");
	    	}

	    }


        memset(cmd,'\0', cmdSize);
        memset(param,'\0', paramSize);
        memset(msg,'\0', msgSize);
	}
}

int main(void) {

	srand(time(0)); //To randomly generate arbitray burst times (1 - 10 units)
	readyQHIGH = ListCreate();
	readyQMED = ListCreate();
	readyQLOW = ListCreate();
	blockedList = ListCreate();
	messageList = ListCreate();
	
	//Initial process that will be the last to terminate at the end of the simulation
	initProc = malloc(sizeof* initProc);
	initProc->id = 0;
	initProc->priority = 0;
	initProc->running = 1;

	//Currently running process, initially will be the same as initProc
	runningProc = malloc(sizeof* runningProc);
	runningProc->id = idCount++;
	runningProc->priority = 0;
	runningProc->running = 1;
	runningProc->burstTime = (rand() % 10) + 1;

	inputCommands();

	free(runningProc);
	free(initProc);
	
	return 0;

}