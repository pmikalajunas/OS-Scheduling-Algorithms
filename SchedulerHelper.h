#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "./Structures.h"
#include "./LinkedList.h"
#include "./BubbleSort.h"

// -------------------- VARIABLES --------------------
int queueLength = 64; // May be resized, if exceded.
int processCount = 0; // Incremented with each input.

// timeElapsed is a simulation of CPU cycles, every iteration is one CPU clock cycle
// 1 CPU Clock Cycle = 1ms
int timeElapsed = 0; 
// ---------------------------------------------------


// -------------------- CONSTANTS --------------------
int const TIME_QUANTUM = 10; // Fixed time quantum.
int const INCREASE_FACTOR = 2; // Factor by which queue length increases.
int const INPUT_ERROR = 1; // Input error code, if the cmd input is incorrect.
int const SUCCESSFUL_EXECUTION = 0; // Returned with successful execution
// ---------------------------------------------------

// -------------- FUNCTION DEFINITIONS ---------------
void printAddedNodeInfo(Node *addedNode);
void printProcessInfo(Process *process);
void printGuidelines();
void printProcessTable(LinkedList *completedQueue);
void printProcessingHeader(Node *node);
void printEmptyQueueError();
void printComparisonData(LinkedList *completedQueue);

Process *newProcess(int burstTime, int arrivalTime);
bool isInt(char* input);
bool verifyAllInputsInt(int argc,  char **argv);
void readCommandLineArguments(int argc, char *argv[], LinkedList *processQueue, LinkedList *waitingQueue);
void discardProcess(Node *node, LinkedList *completedQueue);
void addWaitingNode(LinkedList *waitingQueue, LinkedList *processQueue);
bool isEvenNumberOfArguments(int argc);
int executeSchedulingAlgorithm(LinkedList* (*f)(LinkedList*, LinkedList*, LinkedList*),
                                int argc, char *argv[]);
void putProcessBack(LinkedList *processQueue, Node *node);                                

/////////////////////////////////////////////////////////////////////////////
//
//                          FUNCTIONS
//
/////////////////////////////////////////////////////////////////////////////


// Allocate memory to new process and initialise it.
Process *newProcess(int burstTime, int arrivalTime) {

    // Allocating memory for a process, if available.
    Process *p = (Process *) malloc(sizeof(Process));
    if(p == NULL) {
        return NULL;
    }

    // Set the burst time, pId and return the value.
    p->pId = processCount++;
    p->burstTime = burstTime;
    p->arrivalTime = arrivalTime;
    p->remainingTime = burstTime;
    p->completionTime = 0;
    p->turnAroundTime = 0;
    p->waitingTime = 0;
    p->timeSpentProcessing = 0;

    return p;
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


// Ensure all inputs to the program are integers.
bool verifyAllInputsInt(int argc,  char **argv) {

    // check all int 
    for(int i = 1; i < argc; i++) {
      if(!isInt(argv[i])) {
        printf("(ERROR) All inputs need to be integers");
        return false;
      }
    }
    return true;

}

/**
 * Reads a list of processes with their burst and arrival times from CMD.
 * In the format: (Burst time 1) (Arrival time 1) (Burst time 2) (Arrival time 2) ...
 * Process with arrival time are added to the waiting queue, without - to the processQueue.
 * */
void readCommandLineArguments(int argc, char *argv[], LinkedList *processQueue, LinkedList *waitingQueue) {

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
}



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
    printf("Process (ID: %d) has finished, timeElapsed: (%d)\n",
            node->process->pId, timeElapsed);
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
    printf("Process (ID: %d) appended back to the queue.\n", node->process->pId);
    printProcessInfo(node->process);
}


/**
 * Validates input by checking if we have right amount of arguments.
 * Checks if every character is an integer.
 * Returns INPUT_ERROR if input is user input is not suitable.
 * Returns 0 if input is correct.
 * */
int validateInput(int argc, char *argv[]) {
    if(isEvenNumberOfArguments(argc)) {
        printGuidelines();
        return INPUT_ERROR;
    }

    if(!verifyAllInputsInt(argc,  argv)) return INPUT_ERROR;

    return 0;
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



bool isEvenNumberOfArguments(int argc) {
    return (argc - 1) % 2 != 0;
}


/////////////////////////////////////////////////////////////////////////////
//
//                          PRINTING FUNCTIONS
//
/////////////////////////////////////////////////////////////////////////////


void printComparisonData(LinkedList *completedQueue) {
    ComparisonData data;
    Node *curProcess = peek_head_linked_list(completedQueue);
    float totalNodes = 0,
          totalTurnaroundTime = 0,
          totalWaitingTime = 0,
          totalCompletionTime = 0,
          totalBurstTime = 0,
          totalArrivalTime = 0,
          totalResponseTime;

    do {
        totalNodes++;
        totalTurnaroundTime += curProcess->process->turnAroundTime;
        totalWaitingTime += curProcess->process->waitingTime;
        totalCompletionTime += curProcess->process->completionTime;
        totalBurstTime += curProcess->process->burstTime;
        totalArrivalTime += curProcess->process->arrivalTime;
        totalResponseTime += curProcess->process->responseTime;
        
    } while((curProcess = curProcess->next));

    data.averageTurnaroundTime = totalTurnaroundTime / totalNodes;
    data.averageWaitingTime = totalWaitingTime / totalNodes;
    data.averageCompletionTime = totalCompletionTime / totalNodes;
    data.averageBurstTime = totalBurstTime / totalNodes;
    data.averageArrivalTime = totalArrivalTime / totalNodes;
    data.averageResponseTime = totalResponseTime / totalNodes;

    printf("________________________________________________\n");
    printf("                     Statistics                 \n");
    printf(" Average Turnaround Time = ( %f )\n", data.averageTurnaroundTime);
    printf(" Average Waiting Time = ( %f )\n", data.averageWaitingTime);
    printf(" Average Completion Time = ( %f )\n", data.averageCompletionTime);
    printf(" Average Burst Time = ( %f )\n", data.averageBurstTime);
    printf(" Average Arrival Time = ( %f )\n", data.averageArrivalTime);
    printf(" Average Response Time = ( %f )\n" , data.averageResponseTime);
    printf("________________________________________________\n");

}

/**
 * Prints information about the node which was added from the waiting queue.
 * */
void printAddedNodeInfo(Node *addedNode) {
    printf("\n+++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("\nNode (ID: %d) is added from the waiting queue\n",
            addedNode->process->pId);
    printf("Time elapsed: %d\n", addedNode->process->arrivalTime);
    printf("\n+++++++++++++++++++++++++++++++++++++++++++++++\n");
}

/**
 *  Prints all the information about the process once it finishes executing.
 * */
void printProcessInfo(Process *process) {
    printf("    - Completion time: (%d)\n", process->completionTime);
    printf("    - Turn-around time: (%d)\n", process->turnAroundTime);
    printf("    - Waiting time: (%d)\n", process->waitingTime);
    printf("    - Burst time: (%d)\n", process->burstTime);
    printf("    - Arrival time: (%d)\n", process->arrivalTime);
    printf("    - Time Spent Processing: (%d)\n", process->timeSpentProcessing);
    printf("    - Remaining burst time: (%d)\n", process->remainingTime);
}


/**
 * Prints information on how to use the program, if user uses wrong CMD arguments. 
 * */
void printGuidelines() {
    printf("(ERROR) Invalid parameters.\n");
    printf("Should be in the format of { (Burst time 1) (Arrival time 1) (Burst time 2) (Arrival time 2) ... }\n");
    printf("For example: RR 54 32 2 32 12 2\n");
    printf("Or FCFS 54 32 2 32 12 2\n");
    printf("\n____________________________________________________________________________\n");
}


/**
 * Prints detailed about every process that has been executed.
 * Processes are ordered the way they have been terminated.
 * */
void printProcessTable(LinkedList *completedQueue) {

    printf("\n+-------------------------------------------------------------------------------------+\n");
    printf("|                                  Executed Processes                                 |\n");
    printf("+-----+-----------------+--------------+------------------+------------+--------------+\n");
    printf("|  ID | Completion Time | Waiting Time | Turn-Around Time | Burst Time | Arrival Time |\n");

    // The following line will be used to separate processes from each other.
    char line[] = "+-----+-----------------+--------------+------------------+------------+--------------+\n";
    printf("%s", line);

    Node *node = peek_head_linked_list(completedQueue);

    do {
        printf("|%5d|%17d|%14d|%18d|%12d|%14d|\n",
                node->process->pId, node->process->completionTime,
                node->process->waitingTime, node->process->turnAroundTime,
                node->process->burstTime, node->process->arrivalTime);
        printf("%s", line);
    } while((node = node->next)); 

}


/**
 * Prints process ID and time elapsed.
 * Used in both RR and FCFS to print information on each iteration.
 * */
void printProcessingHeader(Node *node) {
    printf("\ntimeElapsed: (%d)", timeElapsed);
    printf("\n____________________________________________________________________________\n");
    printf("\nProcess (ID: %d) is being processed\n", node->process->pId);
}


/**
 * Informs about the empty processing queue, prints out the elapsed time.
 * */
void printEmptyQueueError() {
    printf("\n__________________________________________________________\n");
    printf("Processing queue is empty, proceeding with another cycle.\n");
    printf("timeElapsed: (%d)\n", timeElapsed);
    printf("\n__________________________________________________________\n");
}