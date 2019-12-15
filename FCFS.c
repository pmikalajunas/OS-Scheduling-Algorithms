#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> 
#include <math.h>
#include <ctype.h>
#include "./LinkedList.h"
#include "./SchedulerHelper.h"

// -------------- FUNCTION DEFINITIONS ---------------
LinkedList* firstComeFirstServe(LinkedList *processQueue, LinkedList *waitingQueue);

// -------------------- FUNCTIONS --------------------

LinkedList* firstComeFirstServe(LinkedList *processQueue, LinkedList *waitingQueue){

    // timeElapsed is a simulation of CPU cycles, every iteration is one CPU clock cycle
    // 1 CPU Clock Cycle = 1ms
    int timeElapsed = 0;
    // Defines how much time we spent executing the current process.
    int timeSpentOnIteration = 0;

    // A list of processes which have finished executing.
    LinkedList *completedQueue = initialise_linked_list();

    while(!linked_list_empty(processQueue) || !linked_list_empty(waitingQueue)){

        // Adding node from waiting queue, before taking one from processQueue.
        addWaitingNode(waitingQueue, processQueue, timeElapsed);

        // Getting the head node of the queue.
        Node *node = remove_head_linked_list(processQueue);

        // If processing queue is empty, we keep on going with another CPU cycle.
        if(!node) {           
            printEmptyQueueError(timeElapsed++);
            continue;
        }

        printProcessingHeader(timeElapsed, node);


        // Retrieving remaining time considering the current iteration.
        int remainingTime = node->process->remainingTime - timeSpentOnIteration;

        // We keep running through CPU cycles while process is being processed.
        // We check if there are nodes in the waiting queue.
        while(remainingTime) {
            timeSpentOnIteration++;
            timeElapsed++;
            remainingTime = node->process->remainingTime - timeSpentOnIteration;
            printf("\ntimeElapsed: (%d), timeSpentOnIteration: (%d), remainingTime: (%d)\n",
             timeElapsed, timeSpentOnIteration, remainingTime);

            addWaitingNode(waitingQueue, processQueue, timeElapsed);
        }
        printf("\n____________________________________________________________________________\n"); 


 

        node->process->timeSpentProcessing = node->process->burstTime;
        discardProcess(node, timeElapsed, completedQueue);
        // Resetting time spent on iteration, next process will take turn.
        timeSpentOnIteration = 0;

    }

    return completedQueue;
}

int main(int argc, char **argv){

    // Process queue will store all the processes without arrival time.
    LinkedList *processQueue = initialise_linked_list();
    // Waiting queue will store all the processes with arrival time.
    LinkedList *waitingQueue = initialise_linked_list();

    if(isEvenNumberOfArguments(argc)) {
        printGuidelines();
        return 1;
    }

    // Ensure all inputs are integers
    if(!verifyAllInputsInt(argc,  argv)) return 1;

    // Processes with arrival time goes to waiting queue, ones without to the processing queue.
    readCommandLineArguments(argc, argv, processQueue, waitingQueue);

    // Sort the list of processes by their arrival time.
    // Allows us to just check the head each time.
    bubbleSort(waitingQueue->head);

    LinkedList *completedQueue = firstComeFirstServe(processQueue, waitingQueue);

    printProcessTable(completedQueue);

    // Free the allocated memory for both queues.
    free_linked_list(processQueue);
    free_linked_list(waitingQueue);
    free_linked_list(completedQueue);
}