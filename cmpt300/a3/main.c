#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"
#include "process.h"

LIST* readyQHIGH;
LIST* readyQMED;
LIST* readyQLOW;
LIST* blockedList;
PCB runningProc;
PCB initProc;
int idCount = 0;

void inputCommands(void) {
	
	while (1) {

		int cmdSize = 4;
		int paramSize = 4;
		int msgSize = 40;
		char cmd[cmdSize];
		char param[paramSize];
		char msg[msgSize];
		printf("Command: ");
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
	    		printf("Command: Fork\n");
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
	    		printf("Command: Quantum\n");

	    	} else if (cmd[0] == 's' || cmd[0] == 'S') {
	    		printf("Send - Please enter the ID of the process to send (blocking)\n");
	    		fgets(param, paramSize, stdin);
	    		int id = atoi(param);
	    		printf("Send - Please enter your message\n");
	    		fgets(msg, msgSize, stdin);
	    		Send(id,msg);



	    		Send(0,NULL);

	    	} else if (cmd[0] == 'r' || cmd[0] == 'R') {
	    		printf("Command: Receive\n");

	    	} else if (cmd[0] == 'y' || cmd[0] == 'Y') {
	    		printf("Command: Reply\n");

	    	} else if (cmd[0] == 'n' || cmd[0] == 'N') {
	    		printf("Command: New Semaphore\n");

	    	} else if (cmd[0] == 'p' || cmd[0] == 'P') {
	    		printf("Command: Semaphore P\n");

	    	} else if (cmd[0] == 'v' || cmd[0] == 'V') {
	    		printf("Command: Semaphore V\n");

	    	} else if (cmd[0] == 'i' || cmd[0] == 'I') {
	    		printf("Command: Process Info\n");

	    	} else if (cmd[0] == 't' || cmd[0] == 'T') {
	    		printf("\nPrinting all system process info...\n");
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


	readyQHIGH = ListCreate();
	readyQMED = ListCreate();
	readyQLOW = ListCreate();
	blockedList = ListCreate();

	PCB runningProc = {idCount++,0,1,NULL};
	initProc = runningProc;

	inputCommands();
	
	return 0;

}