#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "./LinkedList.h"
#include "./SchedulerHelper.h"

// -------------- FUNCTION DEFINITIONS ---------------
LinkedList* roundRobin(LinkedList *processQueue, LinkedList *waitingQueue);
int isReady(Node *node, int currentTimeElapsed);
void discardProcess(Node *node, int timeElapsed, LinkedList *completedQueue);
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
Discards process by assigning completion time and turn around time.
Prints process info and adds process into the queue of completed processes.
*/
void discardProcess(Node *node, int timeElapsed, LinkedList *completedQueue) {
  
    node->process->completionTime = timeElapsed;
    node->process->turnAroundTime = timeElapsed - node->process->arrivalTime;
    printf("Process (ID: %d) has finished, timeElapsed: (%d)\n",
            node->process->pId, timeElapsed);
    printProcessInfo(node->process);
    append_linked_list(completedQueue, node->process);
    free(node);
}


// TODO: we might simulate actual CPU clock ticks
LinkedList* roundRobin(LinkedList *processQueue, LinkedList *waitingQueue) {

    int timeElapsed = 0;
    int count = 0;

    // A list of processes which have finished executing.
    LinkedList *completedQueue = initialise_linked_list();

    // While process queue is not empty.
    while(!linked_list_empty(processQueue)) {

        // Getting the head node of the queue.
        Node *node = remove_head_linked_list(processQueue);

        printf("\nIteration: (%d) timeElapsed: (%d)", count++, timeElapsed);
        printf("\n____________________________________________________________________________\n");
        printf("\nProcess (ID: %d) is being processed\n", node->process->pId);

        // Subtracting TIME_QUANTUM from burst time gives us process' remaining time.
        int burstTimeTemp = node->process->burstTime;
        burstTimeTemp -= TIME_QUANTUM;

        // Waiting time is equal to overall time elapsed minus the time current process spent executing and its arrival time.
        node->process->waitingTime = timeElapsed - node->process->timeSpentProcessing - node->process->arrivalTime;

        // Negative burst time means process finished executing early or exactly on time.
        // Positive burst time means that the process is not finished.
        if(burstTimeTemp > 0) {
            timeElapsed += TIME_QUANTUM;

            // Add the node from the waiting queue (if it's ready).
            addWaitingNode(waitingQueue, processQueue, timeElapsed);

            node->process->timeSpentProcessing += TIME_QUANTUM;
            // Update process burst time by subtracting the time it has been executing.
            node->process->burstTime -= TIME_QUANTUM;

            printf("Process (ID: %d) left CPU (is not finished), timeElapsed: (%d)\n",
                    node->process->pId, timeElapsed);           
            printProcessInfo(node->process);

             // Process is put at the end of the processing queue.
            append_linked_list(processQueue, node->process);
        } else {
            int executionTime = abs(TIME_QUANTUM + burstTimeTemp);
            timeElapsed += executionTime;

            // Add the node from the waiting queue (if it's ready).
            addWaitingNode(waitingQueue, processQueue, timeElapsed);

            node->process->timeSpentProcessing += executionTime;

            discardProcess(node, timeElapsed, completedQueue);
        }


        printf("\n____________________________________________________________________________\n");
    }
    return completedQueue;
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

    // Ensure all inputs are integers
    if(!verifyAllInputsInt(argc,  argv)) return 1;

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

    LinkedList *completedQueue = roundRobin(processQueue, waitingQueue);


    printProcessTable(completedQueue);

    // Free the allocated memory for both queues.
    free_linked_list(processQueue);
    free_linked_list(waitingQueue);
    free_linked_list(completedQueue);
}




