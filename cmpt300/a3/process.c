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
	if (proc->id == 0) //'init' process does not get added to a ready queue, it is always available to run
		return;
	else if (proc->priority == 0)
		ListPrepend(readyQHIGH,proc);
	else if (proc->priority == 1)
		ListPrepend(readyQMED, proc);
	else {
		ListPrepend(readyQLOW, proc);
	}
}

PCB* killProcessFromSys(int pid) {

	if (ListSearch(readyQHIGH,comparator,&pid) != NULL) {
		return ListRemove(readyQHIGH);
	} else if (ListSearch(readyQMED,comparator,&pid) != NULL) {
		return ListRemove(readyQMED);
	} else if (ListSearch(readyQLOW,comparator,&pid) != NULL){
		return ListRemove(readyQLOW);
	} else if ((ListSearch(blockedList, comparator, &pid)) != NULL)
		return ListRemove(blockedList);
	else 
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
	PCB* nextProc;
	if (ListCount(readyQHIGH) != 0) 
		nextProc = ListTrim(readyQHIGH);
	else if (ListCount(readyQMED) != 0)
		nextProc = ListTrim(readyQMED);
	else if (ListCount(readyQLOW) != 0)
		nextProc = ListTrim(readyQLOW);
	else 
		nextProc = initProc;
	
	printf("\nNew running process (ID: %d)\n",nextProc->id);
	return nextProc;
}

//Print out message automatically if proc was blocked on "receiving"
void printReceivedMsg(PCB* proc) {
	if (proc->msg[0] != '\0') {
		printf("\nMessage Received: %s", proc->msg);
		memset(proc->msg, '\0', 40);
	} 
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
		printf("{ID: %d, Time Remaining: %d, SemID: %d, SemVal: %d}\n",currProc->id, currProc->burstTime, currProc->semID, currProc->semVal);
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
		printf("fromID: %d, toID: %d msg: %s", msg->fromID, msg->toID, msg->msg);
		ListPrev(messageList);
	}
}


//END OF HELPER FUNCTIONS

void Create(int priority) {
	PCB* proc = malloc(sizeof *proc);
	proc->id = idCount++;
	proc->priority = priority;
	proc->semID = -1; //No associated semaphore when created
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
	if (pid == 0 && ListCount(readyQLOW) == 0 && ListCount(readyQMED) == 0 && ListCount(readyQHIGH) == 0) { 
		printf("Killing 'init' process, ending simulation\n");
		free(initProc);
		exit(0);
	} else if (runningProc->id == pid) {
		Exit();
	} else {
		PCB* proc = killProcessFromSys(pid);
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
		printReceivedMsg(runningProc);
	} else if (ListCount(readyQLOW) == 0 && ListCount(readyQMED) == 0 && ListCount(readyQHIGH) == 0) {
		printf("\nEnd of simulation\n");
		exit(0);
	} else {
		addToReadyQ(runningProc);
		runningProc = getNextProcess();
		printReceivedMsg(runningProc);
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

	//Unblock any "receiving" process
	PCB* toUnblock;
	if ((toUnblock = ListSearch(blockedList, comparator, &pid)) != NULL) {
		strcpy(toUnblock->msg, msg);
		addToReadyQ(ListRemove(blockedList));
	}

	ListPrepend(messageList, newMsg);
	runningProc = getNextProcess();
	printReceivedMsg(runningProc);

}

void Receive(void) {

	Message* msg = ListSearch(messageList, comparator2, &runningProc->id);

	if (msg == NULL) { 
		if (runningProc->id != 0) {
			PCB* blockedProc = copyProc(runningProc);
			ListPrepend(blockedList, blockedProc);
		} 
		printf("No messages for process (ID: %d)\n",runningProc->id);
		runningProc = getNextProcess();
		printReceivedMsg(runningProc);
	} else {
		printf("Message from (ID: %d) sent to (ID: %d)\n", msg->fromID, msg->toID);
		printf("Message: %s", msg->msg);
		free(ListRemove(messageList));
	}

}


void Reply(int pid, char* msg) {

	//Unblock any "sending" process and reply to it with msg
	PCB* toUnblock;
	if (ListSearch(blockedList, comparator, &pid) != NULL) {
		toUnblock = ListRemove(blockedList);
		addToReadyQ(toUnblock);
		strcpy(toUnblock->msg, msg);
	}


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

	Semaphore* sem = ListSearch(semaphores, comparator, &semID);
	if (sem == NULL) {
		printf("\nNo semaphore found with ID %d\n",semID);
	} else {

		if (runningProc->semID == -1)
			runningProc->semID = semID;

		sem->val--;
		runningProc->semVal = sem->val;
		if (sem->val < 0) {
			PCB* blockedProc = copyProc(runningProc);
			ListPrepend(blockedList, blockedProc);
			runningProc = getNextProcess();
			printReceivedMsg(runningProc);
		}
	}
}
	

void SemaphoreV(int semID) {
	if (semID < 0 || semID > 4) {
		printf("Invalid semaphore ID\n");
		return;
	}

	Semaphore* sem = ListSearch(semaphores, comparator, &semID);
	if (sem == NULL) {
		printf("\nNo semaphore found with ID %d\n",semID);
	} else {

		if (runningProc->semID == -1)
			runningProc->semID = semID;

		PCB* toUnblock;
		sem->val++;
		if (sem->val >= 0) {
			while ((toUnblock = ListSearch(blockedList, comparator3, &semID)) != NULL) {
				printf("Unblocking process (ID: %d)\n", toUnblock->id);
				toUnblock->semVal = sem->val;
				addToReadyQ(ListRemove(blockedList));
			}
		}
	}
}

void ProcessInfo(int pid) {
	PCB* proc = getProcessFromSys(pid);
	if (proc == NULL) {
		printf("\nNo process with ID: %d\n",pid);
	} else
		printf("{ID: %d, Priority: %d, Time Remaining: %d, semID: %d, semVal: %d}\n",proc->id, proc->priority, proc->burstTime, proc->semID, proc->semVal);
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