#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <ctype.h>


// -------------------- VARIABLES --------------------
int queueLength = 64; // May be resized, if exceded.
int processCount = 0; // Incremented with each input.

// timeElapsed is a simulation of CPU cycles, every iteration is one CPU clock cycle
// 1 CPU Clock Cycle = 1ms
int timeElapsed = 0; 


// -------------------- HEADER FILES --------------------
#include "./Debug.h"
#include "./Constants.h"
#include "./Structures.h"
#include "./LinkedList.h"
#include "./BubbleSort.h"
#include "./PrintHelper.h"
#include "./InputHelper.h"


// -------------- FUNCTION DEFINITIONS --------------- 

void discardProcess(Node *node, LinkedList *completedQueue);
void addWaitingNode(LinkedList *waitingQueue, LinkedList *processQueue);
int executeSchedulingAlgorithm(LinkedList* (*f)(LinkedList*, LinkedList*, LinkedList*),
                                int argc, char *argv[]);
void putProcessBack(LinkedList *processQueue, Node *node);                                

/////////////////////////////////////////////////////////////////////////////
//
//                          FUNCTIONS
//
/////////////////////////////////////////////////////////////////////////////


/*
Discards process by assigning completion time and turn around time.
Prints process info and adds process into the queue of completed processes.
*/
void discardProcess(Node *node, LinkedList *completedQueue) {
  
    // We don't want to leave remaining time being negative.
    node->process->remainingTime = 0;

    node->process->completionTime = timeElapsed;
    node->process->turnAroundTime = timeElapsed - node->process->arrivalTime;
    node->process->waitingTime = node->process->turnAroundTime - node->process->burstTime;
    if(DEBUG) {
        printf("Process (ID: %d) has finished, timeElapsed: (%d)\n",
                node->process->pId, timeElapsed);
    }

    printProcessInfo(node->process);
    append_linked_list(completedQueue, node->process);
    free(node);
}

// Checks if the current process is ready to be discarded from waiting queue.
// If arrival time meets the current time, process gets removed and added to the processing queue.
void addWaitingNode(LinkedList *waitingQueue, LinkedList *processQueue) {

    Node *candidateNode = peek_head_linked_list(waitingQueue);

    while(candidateNode && candidateNode->process->arrivalTime <= timeElapsed) {
        candidateNode = remove_head_linked_list(waitingQueue);
        append_linked_list(processQueue, candidateNode->process);

        printAddedNodeInfo(candidateNode);
        candidateNode = peek_head_linked_list(waitingQueue);
    }
}


/**
 * Puts process back to the processing queue.
 * Used in RoundRobin to achieve differrent functionality in MLFQ.
 * Prints info about the process.
 * */
void putProcessBack(LinkedList *processQueue, Node *node) {
    append_linked_list(processQueue, node->process);
    if(DEBUG) {
        printf("Process (ID: %d) appended back to the queue.\n", node->process->pId);
    }
    printProcessInfo(node->process);
}


/**
 * Executes passed scheduling algorithm function with given CMD arguments (argv).
 * Prints the table of executed processes in their order of completion.
 * (argc, argv) are the command line arguments.
 * f - is the scheduling algorithm function that will operate on process and waiting queue.
 * */
int executeSchedulingAlgorithm(LinkedList* (*f)(LinkedList*, LinkedList*, LinkedList*),
                                int argc, char *argv[]) 
{

    if(validateInput(argc, argv) == INPUT_ERROR) {
        return INPUT_ERROR;
    }


    // Process queue will store all the processes without arrival time.
    LinkedList *processQueue = initialise_linked_list();
    // Waiting queue will store all the processes with arrival time.
    LinkedList *waitingQueue = initialise_linked_list();

    // Processes with arrival time goes to waiting queue, ones without to the processing queue.
    readCommandLineArguments(argc, argv, processQueue, waitingQueue);

    // Sort the list of processes by their arrival time.
    // Allows us to just check the head each time.
    bubbleSort(waitingQueue->head);

    LinkedList *completedQueue = (*f)(processQueue, waitingQueue, processQueue);

    printProcessTable(completedQueue);

    printComparisonData(completedQueue);

    // Free the allocated memory for both queues.
    free_linked_list(processQueue);
    free_linked_list(waitingQueue);
    free_linked_list(completedQueue);

    return SUCCESSFUL_EXECUTION;
}



