#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "./LinkedList.h"

// -------------------- CONSTANTS --------------------
int const TIME_QUANTUM = 100; // Fixed time quantum.
int const INCREASE_FACTOR = 2; // Factor by which queue length increases.
// ---------------------------------------------------

// -------------------- VARIABLES --------------------
int queueLength = 64; // May be resized, if exceded.
int processCount = 0; // Incremented with each input
// ---------------------------------------------------

// -------------- FUNCTION DEFINITIONS ---------------
void roundRobin(LinkedList *processQueue, LinkedList *waitingQueue);
void printProcessInfo(Process *process);
Process *newProcess(int burstTime, int arrivalTime);
void memCheck(void *ptr);
bool isInt(char*);
void printProcessInfo(Process *process);
int isReady(Node *node, int currentTimeElapsed);
void discardProcess(Node *node, int timeElapsed);
void printProcessedNodeData(Node *node, int timeElapsed);
void printGuidelines();
void addWaitingNode(LinkedList *waitingQueue, LinkedList *processQueue, int timeElapsed);

// -------------------- FUNCTIONS --------------------


/*
Tells if the prcoess is ready to be executed.
Measures arrival time against elapsed time.
Returns 0 if it's not ready.
Returns 1 if the process is ready.
*/
int isReady(Node *node, int currentTimeElapsed) {

    int arrivalTime = node->process->arrivalTime;

    if(arrivalTime <= currentTimeElapsed) {
        return 1;
    }
    return 0;
}

/*
Discards process by assigning completion time and freeing allocated memory.
Prints process info.
*/
void discardProcess(Node *node, int timeElapsed) {
    node->process->completionTime = timeElapsed;
    printProcessInfo(node->process);
    free(node);
}


void printProcessedNodeData(Node *node, int timeElapsed) {
    printf("\nProcess (ID: %d) left CPU, it has been processed for %ds \n",
           node->process->pId, TIME_QUANTUM);
    printf("Remaining burst time: %ds\n", node->process->burstTime);
    printf("Process spent %ds in total\n", node->process->timeSpentProcessing);
    printf("Elapsed time till now: %ds\n", timeElapsed);
}

// Checks if the current process is ready to be discarded from waiting queue.
// If arrival time meets the current time, process gets removed and added to processing queue.
void addWaitingNode(LinkedList *waitingQueue, LinkedList *processQueue, int timeElapsed) {

    Node *candidateNode = peek_head_linked_list(waitingQueue);
    while(candidateNode && candidateNode->process->arrivalTime <= timeElapsed) {
        candidateNode = remove_head_linked_list(waitingQueue);
        append_linked_list(processQueue, candidateNode->process);
        printf("\nNode (ID: %d) is added from the waiting queue\n",
                candidateNode->process->pId);
        printf("Time elapsed: %d\n", candidateNode->process->arrivalTime);
        candidateNode = peek_head_linked_list(waitingQueue);
    }
}

// TODO: we might simulate actual CPU clock ticks
void roundRobin(LinkedList *processQueue, LinkedList *waitingQueue) {

    int timeElapsed = 0;
    int count = 0;

    // While process queue is not empty.
    while(!linked_list_empty(processQueue)) {

        // Getting the head node of the queue.
        Node *node = remove_head_linked_list(processQueue);

        printf("Iteration: (%d) timeElapsed: (%d)", count++, timeElapsed);
        printf("\n____________________________________________________________________________\n");
        printf("\nProcess (ID: %d) is being processed\n", node->process->pId);

        // Subtracting TIME_QUANTUM from burst time gives us process' remaining time.
        int burstTimeTemp = node->process->burstTime;
        burstTimeTemp -= TIME_QUANTUM;

        // Waiting time is equal to overall time elapsed minus the time current process spent executing.
        node->process->waitingTime = timeElapsed - node->process->timeSpentProcessing;

        // Negative burst time means process finished executing early or exactly on time.
        // Positive burst time means that the process is not finished.
        if(burstTimeTemp > 0) {
            timeElapsed += TIME_QUANTUM;

            // Add the node from the waiting queue (if it's ready).
            addWaitingNode(waitingQueue, processQueue, timeElapsed);

            node->process->timeSpentProcessing += TIME_QUANTUM;
            // Update process burst time by subtracting the time it has been executing.
            node->process->burstTime -= TIME_QUANTUM;

            printProcessedNodeData(node, timeElapsed);

             // Process is put at the end of the processing queue.
            append_linked_list(processQueue, node->process);
        } else {
            int executionTime = abs(TIME_QUANTUM + burstTimeTemp);
            timeElapsed += executionTime;

            // Add the node from the waiting queue (if it's ready).
            addWaitingNode(waitingQueue, processQueue, timeElapsed);

            node->process->timeSpentProcessing += executionTime;

            discardProcess(node, timeElapsed);
        }


        printf("\n____________________________________________________________________________\n");
    }

}

/* Prints all the information about the process. */
void printProcessInfo(Process *process) {
    printf("Process (%d) has finshed executing\n\n", process->pId);
    printf("    - Completion time: (%d)\n", process->completionTime);
    printf("    - Turn-around time: (%d)\n", process->turnAroundTime);
    printf("    - Waiting time: (%d)\n", process->waitingTime);
    printf("    - Burst time: (%d)\n", process->burstTime);
    printf("    - Arrival time: (%d)\n", process->arrivalTime);
    printf("    - Time Spent Processng: (%d)\n", process->timeSpentProcessing);
}

// Allocate memory to new process and initialise it.
Process *newProcess(int burstTime, int arrivalTime) {

    // Allocating memory for a process, if available.
    Process *p = malloc(sizeof(Process));
    if(p == NULL) {
        return NULL;
    }

    // Set the burst time, pId and return the value.
    p->pId = processCount++;
    p->burstTime = burstTime;
    p->arrivalTime = arrivalTime;
    return p;
}

// Check if dynamic memory allocation was successful.
// Exits the program and prints an error message if it wasn't.
void memCheck(void *ptr) {
    if(ptr == NULL) {
        printf("(ERROR) Failed to allocate/reallocate memory!\n");
        exit(0);
    }
}


// Check if input string contains all integers.
// Return true if all digits, false if not.
bool isInt(char* input) {
    for(int i = 0; i < strlen(input); i++) {
        if(!isdigit(input[i])) {
            return false;
        }
    }
    return true;
}

void printGuidelines() {
    printf("(ERROR) Invalid parameters.\n");
    printf("Should be in the format of { (Burst time 1) (Arrival time 1) (Burst time 2) (Arrival time 2) ... }\n");
    printf("For example: RR 54 32 2 32 12 2\n");
}

bool isEvenNumberOfArguments(int argc) {
    return (argc - 1) % 2 != 0;
}

int main(int argc, char *argv[]) {

    if(isEvenNumberOfArguments(argc)) {
        printGuidelines();
        return 1;
    }

    // Process queue will store all the processes without arrival time.
    LinkedList *processQueue = initialise_linked_list();
    // Waiting queue will store all the processes with arrival time.
    LinkedList *waitingQueue = initialise_linked_list();

    // Check if every entry is an integer.
    for(int i = 1; i < argc; i++) {
        if(!isInt(argv[i])) {
            printf("(ERROR) All inputs need to be integers");
            return 1;
        }
    }

    // Processes with arrival time goes to waiting queue, ones without to processing queue.
    for(int i = 1; i < argc; i += 2) {
        char *ptr;
        int burstTime = (int) strtol(argv[i], &ptr, 10);
        int arrivalTime = (int) strtol(argv[i + 1], &ptr, 10);

        if(arrivalTime == 0) {
            append_linked_list(processQueue, newProcess(burstTime, arrivalTime));
        } else {
            append_linked_list(waitingQueue, newProcess(burstTime, arrivalTime));
        }

    }

    roundRobin(processQueue, waitingQueue);

    // Free the allocated memory for both queues.
    free_linked_list(processQueue);
    free_linked_list(waitingQueue);
}
