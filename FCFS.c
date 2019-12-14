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

    int timeElapsed = 0;
    int count = 0;

    // A list of processes which have finished executing.
    LinkedList *completedQueue = initialise_linked_list();

    while(!linked_list_empty(processQueue)){

        // Getting the head node of the queue.
        Node *node = remove_head_linked_list(processQueue);

        printf("\nIteration: (%d) timeElapsed: (%d)", count++, timeElapsed);
        printf("\n____________________________________________________________________________\n");
        printf("\nProcess (ID: %d) is being processed\n", node->process->pId);
        

        // Calculating waiting time and the total time elapsed.
   //  node->process->waitingTime = timeElapsed - node->process->arrivalTime;
        timeElapsed += node->process->burstTime;

        // Add the node from the waiting queue (if it's ready).
        addWaitingNode(waitingQueue, processQueue, timeElapsed);

        node->process->timeSpentProcessing = node->process->burstTime;

        node->process->completionTime = timeElapsed;
        node->process->turnAroundTime = node->process->completionTime - node->process->arrivalTime;
        
        node->process->waitingTime = node->process->turnAroundTime - node->process->burstTime;

        printf("Process (ID: %d) has finished, timeElapsed: (%d)\n",
              node->process->pId, timeElapsed);
        printProcessInfo(node->process);

        append_linked_list(completedQueue, node->process);

        free(node);

        printf("\n____________________________________________________________________________\n");
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

    // Append all inputs to linked list.
    for(int i = 1; i < argc; i += 2) {
        char *ptr;
        int burstTime = strtol(argv[i], &ptr, 10);
        int arrivalTime = strtol(argv[i + 1], &ptr, 10);
        
        append_linked_list(processQueue, newProcess(burstTime, arrivalTime));
    }

    LinkedList *completedQueue = firstComeFirstServe(processQueue, waitingQueue);

    printProcessTable(completedQueue);

    // Free the allocated memory for both queues.
    free_linked_list(processQueue);
    free_linked_list(waitingQueue);
    free_linked_list(completedQueue);
}