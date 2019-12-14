#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "./LinkedList.h"
#include "./SchedulerHelper.h"
#include "./BubbleSort.h"

// -------------- FUNCTION DEFINITIONS ---------------
LinkedList* roundRobin(LinkedList *processQueue, LinkedList *waitingQueue);
int isReady(Node *node, int currentTimeElapsed);
void discardProcess(Node *node, int timeElapsed, LinkedList *completedQueue);

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
  
    // We don't want to leave remaining time being negative.
    node->process->remainingTime = 0;

    node->process->completionTime = timeElapsed;
    node->process->turnAroundTime = timeElapsed - node->process->arrivalTime;
    node->process->waitingTime = node->process->turnAroundTime - node->process->burstTime;
    printf("Process (ID: %d) has finished, timeElapsed: (%d)\n",
            node->process->pId, timeElapsed);
    printProcessInfo(node->process);
    append_linked_list(completedQueue, node->process);
    free(node);
}


LinkedList* roundRobin(LinkedList *processQueue, LinkedList *waitingQueue) {

    // timeElapsed is a simulation of CPU cycles, every iteration is one CPU clock cycle
    int timeElapsed = 0;
    // Defines how much time we spent executing the current process.
    int timeSpentOnIteration = 0;

    // A list of processes which have finished executing.
    LinkedList *completedQueue = initialise_linked_list();

    // While both process queue and waiting queue are not empty.
    while(!linked_list_empty(processQueue) || !linked_list_empty(waitingQueue)) {

        // Add the node from the waiting queue (if it's ready).
        addWaitingNode(waitingQueue, processQueue, timeElapsed);

        // Getting the head node of the queue.
        Node *node = remove_head_linked_list(processQueue);

        // If processing queue is empty, we keep on going with another CPU cycle.
        if(!node) {           
            printf("\n__________________________________________________________\n");
            printf("Processing queue is empty, proceeding with another cycle.\n");
            printf("timeElapsed: (%d)\n", timeElapsed);
            printf("\n__________________________________________________________\n");
            timeElapsed++;
            continue;
        }

        printProcessingHeader(timeElapsed, node);

        // Retrieving remaining time considering the current iteration.
        int remainingTime = node->process->remainingTime - timeSpentOnIteration;

        // We execute current process and add nodes from the waiting queue in the meantime.
        while(remainingTime && (TIME_QUANTUM > timeSpentOnIteration)) {
            timeSpentOnIteration++;
            timeElapsed++;
            remainingTime = node->process->remainingTime - timeSpentOnIteration;
            printf("\ntimeElapsed: (%d), timeSpentOnIteration: (%d), remainingTime: (%d)\n",
             timeElapsed, timeSpentOnIteration, remainingTime);

            addWaitingNode(waitingQueue, processQueue, timeElapsed);
        }
        printf("\n____________________________________________________________________________\n"); 
            

        node->process->timeSpentProcessing += timeSpentOnIteration;


        // Waiting time is equal to overall time elapsed minus the time current process spent executing and its arrival time.
        //node->process->waitingTime = timeElapsed - node->process->timeSpentProcessing - node->process->arrivalTime;

        // Updating process' remaining time by subtracrting the time it spent executing.
        node->process->remainingTime -= timeSpentOnIteration;

        printProcessInfo(node->process);

        // If process is not done yet add it back to the processing queue.
        if(node->process->remainingTime > 0) {
            append_linked_list(processQueue, node->process);
            printf("Process (ID: %d) appended back to the queue.\n", node->process->pId);
        } else {


            discardProcess(node, timeElapsed, completedQueue);
        }


        // Resetting time spent on iteration, next process will take turn.
        timeSpentOnIteration = 0;

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

    bubbleSort(waitingQueue->head);

    LinkedList *completedQueue = roundRobin(processQueue, waitingQueue);

    printProcessTable(completedQueue);

    // Free the allocated memory for both queues.
    free_linked_list(processQueue);
    free_linked_list(waitingQueue);
    free_linked_list(completedQueue);
}




