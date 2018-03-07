#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "process.h"

extern LIST* readyQHIGH;
extern LIST* readyQMED;
extern LIST* readyQLOW;
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
	PCB* process = NULL;
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
	printf("New process (ID: %d) is now running\n",runningProc.id);
}

void Quantum(void) {

}


void Send(int pid, char* msg) {

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
	printf("\n");
}