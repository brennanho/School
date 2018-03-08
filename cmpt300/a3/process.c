#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "process.h"

extern LIST* readyQHIGH;
extern LIST* readyQMED;
extern LIST* readyQLOW;
extern LIST* blockedList;
extern PCB runningProc;
extern int idCount;

//HELPER FUNCTIONS

void addToReadyQ(PCB* proc) {
	if (proc->priority == 0)
		ListPrepend(readyQHIGH,proc);
	else if (proc->priority == 1)
		ListPrepend(readyQMED, proc);
	else //priority == 2
		ListPrepend(readyQLOW, proc);
}

PCB* killProcessFromQ(int pid) {
	int* pidArg = malloc(sizeof* pidArg);
	*pidArg = pid;

	if (ListSearch(readyQHIGH,comparator,pidArg) != NULL) {
		free(pidArg);
		return ListRemove(readyQHIGH);
	} else if (ListSearch(readyQMED,comparator,pidArg) != NULL) {
		free(pidArg);
		return ListRemove(readyQMED);
	} else if (ListSearch(readyQLOW,comparator,pidArg) != NULL){
		free(pidArg);
		return ListRemove(readyQLOW);
	}

	free(pidArg);
	return NULL;
}

PCB* getProcessFromQ(int pid) {
	PCB* process;
	int* pidArg = malloc(sizeof* pidArg);
	*pidArg = pid;

	if ((process = ListSearch(readyQHIGH,comparator,pidArg)) != NULL) {
		free(pidArg);
		return process;
	} else if ((process = ListSearch(readyQMED,comparator,pidArg)) != NULL) {
		free(pidArg);
		return process;
	} else if ((process = ListSearch(readyQLOW,comparator,pidArg)) != NULL){
		free(pidArg);
		return process;
	}

	free(pidArg);
	return NULL;
}

PCB* getNextProcess() {
	if (ListCount(readyQHIGH) != 0)
		return ListTrim(readyQHIGH);
	else if (ListCount(readyQMED) != 0)
		return ListTrim(readyQMED);
	else if (ListCount(readyQLOW) != 0)
		return ListTrim(readyQLOW);
	else 
		printf("No processes currently waiting\n");
	return NULL;
}

void printQ(LIST* Q) {

	if (ListCount(Q) == 0) {
		printf("No processes waiting\n");
	}

	ListFirst(Q);
	PCB* currProc = ListFirst(Q);
	while (currProc != NULL) {
		printf("{ID: %d, Priority: %d, Running: %d, Message: %s}\n",currProc->id, currProc->priority, currProc->running, currProc->procMessage);
		currProc = ListNext(Q);
	}
}


//END OF HELPER FUNCTIONS

void Create(int priority) {
	PCB* proc = malloc(sizeof *proc);
	proc->id = idCount++;
	proc->priority = priority;
	proc->running = 0;
	proc->procMessage = NULL;
	addToReadyQ(proc);

}

void Fork(void) {
	if (runningProc.id != 0) {
		PCB* proc = malloc(sizeof *proc);
		proc->id = idCount++;
		proc->priority = runningProc.priority;
		proc->procMessage = runningProc.procMessage;
		proc->running = 0;
		addToReadyQ(proc);
		printf("Success: new process id is %d",proc->id);

	} else {
		printf("Error: attempting to fork 'init' process\n");
	}
}

void Kill(int pid) {
	if (pid == 0 && ListCount(readyQLOW) == 0 && ListCount(readyQMED) == 0 && ListCount(readyQHIGH) == 0) 
		printf("Killing 'init' process\n");
	else if (runningProc.id = pid) {
		Exit();
	}
	else {
		PCB* proc = killProcessFromQ(pid);
		if (proc == NULL)
			printf("Process (ID: %d) not found in system\n",pid);
		else {
			printf("Process (ID: %d) successfully killed\n",pid);
			free(proc);
		}
	}

}

void Exit(void) {
	PCB* nextProc = getNextProcess();
	if (nextProc == NULL)
		return;
	runningProc.id = nextProc->id;
	runningProc.procMessage = nextProc->procMessage;
	runningProc.priority = nextProc->priority;
	free(nextProc);
	printf("New process (ID: %d) is now running\n",runningProc.id);
}

void Quantum(void) {

}


void Send(int pid, char* msg) {
	PCB* blockedProc = malloc(sizeof* blockedProc);
	blockedProc->id = runningProc.id;
	blockedProc->procMessage = runningProc.procMessage;
	blockedProc->priority = runningProc.priority;
	blockedProc->running = 0; 
	ListPrepend(blockedList,blockedProc);

	PCB* sendTo = getProcessFromQ(pid);
	if (sendTo == NULL)
		return;
	
	sendTo->procMessage = msg;

	PCB* nextProc = getNextProcess();
	if (nextProc == NULL)
		return;

	runningProc.id = nextProc->id;
	runningProc.procMessage = nextProc->procMessage;
	runningProc.priority = nextProc->priority;
	runningProc.running = 1;

}


void Receive(void) {

}


void Reply(int pid, char* msg) {

}

void NewSemaphore(int semID, int initVal) {

}


void SemaphoreP(int semID) {

}


void SemaphoreV(int semID) {

}

void ProcessInfo(int pid) {

}

void TotalInfo(void) {
	printf("\nReady Queue 0:\n");
	printQ(readyQHIGH);
	printf("\nReady Queue 1:\n");
	printQ(readyQMED);
	printf("\nReady Queue 2:\n");
	printQ(readyQLOW);
	printf("\nBlocked Processes:\n");
	printQ(blockedList);
	printf("\n");
}