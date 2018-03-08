#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "process.h"

extern LIST* readyQHIGH;
extern LIST* readyQMED;
extern LIST* readyQLOW;
extern LIST* blockedList;
extern LIST* messageList;
extern PCB* runningProc;
extern PCB* initProc;
extern int idCount;
extern int quantum;

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

PCB* getProcessFromSys(int pid) {
	PCB* process;
	int* pidArg = malloc(sizeof* pidArg);
	*pidArg = pid;

	if (runningProc->id == pid) {
		free(pidArg);
		return runningProc;
	} else if ((process = ListSearch(readyQHIGH,comparator,pidArg)) != NULL) {
		free(pidArg);
		return process;
	} else if ((process = ListSearch(readyQMED,comparator,pidArg)) != NULL) {
		free(pidArg);
		return process;
	} else if ((process = ListSearch(readyQLOW,comparator,pidArg)) != NULL){
		free(pidArg);
		return process;
	} else if ((process = ListSearch(blockedList,comparator,pidArg)) != NULL) {
		free(pidArg);
		return process;
	}

	free(pidArg);
	return NULL;
}

PCB* becomeBlocked() {
	PCB* blockedProc = malloc(sizeof* blockedProc);
	blockedProc->id = runningProc->id;
	blockedProc->msg = runningProc->msg;
	blockedProc->priority = runningProc->priority;
	ListPrepend(blockedList,blockedProc);
	return blockedProc;
}

PCB* getNextProcess() {
	if (ListCount(readyQHIGH) != 0)
		return ListTrim(readyQHIGH);
	else if (ListCount(readyQMED) != 0)
		return ListTrim(readyQMED);
	else if (ListCount(readyQLOW) != 0)
		return ListTrim(readyQLOW);
	else 
		return initProc;
}

void aquireCPU(PCB* nextProc) {
	runningProc->id = nextProc->id;
	runningProc->msg = nextProc->msg;
	runningProc->priority = nextProc->priority;
	runningProc->burstTime = nextProc->burstTime;
}

PCB* copyProc(PCB* proc) {
	PCB* copyProc = malloc(sizeof* copyProc);
	copyProc->msg = proc->msg;
	copyProc->id = proc->id;
	copyProc->priority = proc->priority;
	copyProc->burstTime = proc->burstTime;
	return copyProc;
}

void printQ(LIST* Q) {

	if (ListCount(Q) == 0) {
		printf("No processes waiting\n");
	}

	ListFirst(Q);
	PCB* currProc = ListLast(Q);
	while (currProc != NULL) {
		printf("{ID: %d, Time Remaining: %d, Message: %s}\n",currProc->id, currProc->burstTime, currProc->msg);
		currProc = ListPrev(Q);
	}
}


//END OF HELPER FUNCTIONS

void Create(int priority) {
	PCB* proc = malloc(sizeof *proc);
	proc->id = idCount++;
	proc->priority = priority;
	proc->burstTime = (rand() % 10) + 1; //Arbitrary
	proc->msg = NULL;
	addToReadyQ(proc);

}

void Fork(void) {
	if (runningProc->id != initProc->id) {
		PCB* proc = malloc(sizeof *proc);
		proc->id = idCount++;
		proc->priority = runningProc->priority;
		proc->msg = runningProc->msg;
		addToReadyQ(proc);
		printf("Success: new process id is %d",proc->id);

	} else {
		printf("Error: attempting to fork 'init' process\n");
	}
}

void Kill(int pid) {
	if (pid == 0 && ListCount(readyQLOW) == 0 && ListCount(readyQMED) == 0 && ListCount(readyQHIGH) == 0) 
		printf("Killing 'init' process\n");
	else if (runningProc->id = pid) {
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
	printf("New process (ID: %d) is now running\n", nextProc->id);
	if (nextProc->id == 0) {
		aquireCPU(nextProc);
	} else {
		aquireCPU(nextProc);
		free(nextProc);
	}
}

void Quantum(void) {
	if (runningProc == NULL)
		return;
	else {
		runningProc->burstTime = runningProc->burstTime - quantum;
		if (runningProc->burstTime <= 0) {
			Exit();
		} else {
			addToReadyQ(copyProc(runningProc));
			Exit();
		}
	}
}


void Send(int pid, char* msg) {
	PCB* blockedProc = becomeBlocked();
	Message* newMsg = malloc(sizeof* newMsg);
	newMsg->fromID = blockedProc->id;
	newMsg->toID = pid;
	newMsg->msg = msg;
	ListPrepend(messageList, newMsg);

	PCB* nextProc = getNextProcess();
	if (nextProc == NULL)
		return;

	aquireCPU(nextProc);

}


void Receive(void) {
	PCB* blockedProc = becomeBlocked();
	int* blockedID = malloc(sizeof *blockedID);
	*blockedID = blockedProc->id;
	Message* msg = ListSearch(messageList,comparator2,blockedID);

	if (msg == NULL)
		printf("No messages for process (ID: %d)\n",*blockedID);
	else {
		ListRemove(messageList);
		printf("Message from (ID: %d) sent to (ID: %d)\n", msg->fromID, msg->toID);
		printf("Message: %s\n", msg->msg);
		blockedProc->msg = msg->msg;
	}

	free(blockedID);
	PCB* nextProc = getNextProcess();
	if (nextProc == NULL)
		return;
	aquireCPU(nextProc);

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
	PCB* proc = getProcessFromSys(pid);
	if (proc == NULL) {
		printf("\nNo process with ID: %d\n",pid);
		return;
	}

	printf("{ID: %d, Priority: %d, Time Remaining: %d, Message: %s}\n",proc->id, proc->priority, proc->burstTime, proc->msg);

}

void TotalInfo(void) {
	printf("\nPrinting all system process info...\n");
	printf("\nRunning process:\n");
	ProcessInfo(runningProc->id);
	printf("\nReady Queue High Priority:\n");
	printQ(readyQHIGH);
	printf("\nReady Queue Medium Priority:\n");
	printQ(readyQMED);
	printf("\nReady Queue Low Priority:\n");
	printQ(readyQLOW);
	printf("\nBlocked Processes:\n");
	printQ(blockedList);
	printf("\n");
}