#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "process.h"

extern LIST* readyQHIGH;
extern LIST* readyQMED;
extern LIST* readyQLOW;
extern LIST* blockedList;
extern LIST* messageList;
extern LIST* semaphores;
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

	if (ListSearch(readyQHIGH,comparator,&pid) != NULL) {
		return ListRemove(readyQHIGH);
	} else if (ListSearch(readyQMED,comparator,&pid) != NULL) {
		return ListRemove(readyQMED);
	} else if (ListSearch(readyQLOW,comparator,&pid) != NULL){
		return ListRemove(readyQLOW);
	}
	return NULL;
}

PCB* getProcessFromSys(int pid) {
	PCB* process;
	if (runningProc->id == pid) {
		return runningProc;
	} else if ((process = ListSearch(readyQHIGH,comparator,&pid)) != NULL) {
		return process;
	} else if ((process = ListSearch(readyQMED,comparator,&pid)) != NULL) {
		return process;
	} else if ((process = ListSearch(readyQLOW,comparator,&pid)) != NULL){
		return process;
	} else if ((process = ListSearch(blockedList,comparator,&pid)) != NULL) {
		return process;
	}

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
		return initProc;
}

PCB* copyProc(PCB* proc) {
	PCB* copyProc = malloc(sizeof* copyProc);
	strcpy(copyProc->msg, proc->msg);
	copyProc->semID = proc->semID;
	copyProc->id = proc->id;
	copyProc->semVal = proc->semVal;
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
		printf("{ID: %d, Time Remaining: %d, Message: %s, SemID: %d, SemVal: %d}\n",currProc->id, currProc->burstTime, currProc->msg, currProc->semID, currProc->semVal);
		currProc = ListPrev(Q);
	}
}

void printMessageList(LIST* messageList) {
	if (ListCount(messageList) == 0) {
		printf("No messages in message list\n");
		return;
	}
	ListLast(messageList);
	while (messageList->curr != NULL) {
		Message* msg = ListCurr(messageList);
		printf("{msg: %s, fromID: %d, toID: %d}\n",msg->msg, msg->fromID, msg->toID);
		ListPrev(messageList);
	}
}


//END OF HELPER FUNCTIONS

void Create(int priority) {
	PCB* proc = malloc(sizeof *proc);
	proc->id = idCount++;
	proc->priority = priority;
	proc->burstTime = (rand() % 10) + 1; //Arbitrary
	addToReadyQ(proc);

}

void Fork(void) {
	if (runningProc->id != 0) {
		PCB* proc = copyProc(runningProc);
		proc->id = idCount++;
		addToReadyQ(proc);
		printf("Success: new process id is %d",proc->id);

	} else {
		printf("Error: attempting to fork 'init' process\n");
	}
}

void Kill(int pid) {
	if (pid == 0 && ListCount(readyQLOW) == 0 && ListCount(readyQMED) == 0 && ListCount(readyQHIGH) == 0) 
		printf("Killing 'init' process\n");
	else if (runningProc->id == pid) {
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
	if (runningProc->id != 0) {
		free(runningProc);
		runningProc = getNextProcess();
		printf("\nNew process (ID: %d) is now running\n", runningProc->id);
	} else if (ListCount(readyQLOW) == 0 && ListCount(readyQMED) == 0 && ListCount(readyQHIGH) == 0) {
		printf("\nEnd of simulation\n");
		exit(0);
	} else {
		addToReadyQ(runningProc);
		runningProc = getNextProcess();
	}
}

void Quantum(void) {
	runningProc->burstTime = runningProc->burstTime - quantum;
	if (runningProc->burstTime <= 0) {
		Exit();
	} else {
		PCB* preemptProc = copyProc(runningProc);
		addToReadyQ(preemptProc);
		Exit();
	}
}


void Send(int pid, char* msg) {
	Message* newMsg = malloc(sizeof* newMsg);
	newMsg->fromID = runningProc->id;
	newMsg->toID = pid;
	strcpy(newMsg->msg, msg);

	//Do not block the running process if it is the init process
	if (runningProc->id != 0) {
		PCB* blockedProc = copyProc(runningProc);
		ListPrepend(blockedList, blockedProc);
	} else {
		addToReadyQ(runningProc);
	}

	ListPrepend(messageList, newMsg);
	runningProc = getNextProcess();

}

void Receive(void) {
	Message* msg = ListSearch(messageList,comparator2,&(runningProc->id));

	if (msg == NULL) {
		
		if (runningProc->id == 0)
			return;

		PCB* blockedProc = copyProc(runningProc);
		ListPrepend(blockedList, blockedProc);
		printf("No messages for process (ID: %d)\n",blockedProc->id);
		runningProc = getNextProcess();

	} else {
		printf("Message from (ID: %d) sent to (ID: %d)\n", msg->fromID, msg->toID);
		printf("Message: %s\n", msg->msg);
		strcpy(runningProc->msg, msg->msg);
		ListRemove(messageList);
	}

}


void Reply(int pid, char* msg) {

}

void NewSemaphore(int semID, int initVal) {
	Semaphore* newSem = malloc(sizeof *newSem);
	newSem->id = semID;
	newSem->val = initVal;
	ListPrepend(semaphores,newSem);
}


void SemaphoreP(int semID) {
	if (semID < 0 || semID > 4) {
		printf("Invalid semaphore ID\n");
		return;
	}
	int* ID = malloc(sizeof *ID);
	*ID = semID;
	Semaphore* sem = ListSearch(semaphores, comparator, ID);
	if (sem == NULL) {
		printf("\nNo semaphore found with ID %d\n",semID);
	} else {
		sem->val--;
		runningProc->semVal = sem->val;
		if (sem->val < 0) {
			PCB* blockedProc = copyProc(runningProc);
			ListPrepend(blockedList, blockedProc);
			runningProc = getNextProcess();
		}
	}
}


void SemaphoreV(int semID) {
	if (semID < 0 || semID > 4) {
		printf("Invalid semaphore ID\n");
		return;
	}
	int* ID = malloc(sizeof *ID);
	*ID = semID;
	Semaphore* sem = ListSearch(semaphores, comparator, ID);
	if (sem == NULL) {
		printf("\nNo semaphore found with ID %d\n",semID);
	} else {
		sem->val++;
		if (sem->val >= 0) {
			while (ListSearch(blockedList, comparator3, ID) != NULL) {
				printf("Found process\n");
				addToReadyQ(ListRemove(blockedList));
			}
		}
	}
	free(ID);
}

void ProcessInfo(int pid) {
	PCB* proc = getProcessFromSys(pid);
	if (proc == NULL) {
		printf("\nNo process with ID: %d\n",pid);
	} else
		printf("{ID: %d, Priority: %d, Time Remaining: %d, Message: %s, semID: %d, semVal: %d}\n",proc->id, proc->priority, proc->burstTime, proc->msg, proc->semID, proc->semVal);
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
	printf("\nMessage List:\n");
	printMessageList(messageList);
	printf("\n");
}