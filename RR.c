#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> 
#include <math.h>
#include <ctype.h>
#include "./LinkedList.h"

// -------------------- CONSTANTS -------------------- 
int const TIME_QUANTUM = 1; // Fixed time quantum.
int const INCREASE_FACTOR = 2; // Factor by which queue length increases.
// --------------------------------------------------- 

// -------------------- VARIABLES -------------------- 
int queueLength = 64; // May be resized, if exceded.
int processCount = 0; // Incremented with each input
// --------------------------------------------------- 

// -------------- FUNCTION DEFINITIONS ---------------
void roundRobin(LinkedList *queue);
void printProcessInfo(Process *process);
Process *newProcess(int burstTime, int arrivalTime);
void memCheck(void *ptr);
bool isInt(char*);
void printProcessInfo(Process *process);

// -------------------- FUNCTIONS --------------------

void roundRobin(LinkedList *queue) {

    int timeElapsed = 0;

    // While process queue is not empty.
    while(!linked_list_empty(queue)) {

        // Check if a process is ready to be executed (arrival time)


        // Getting the head node of the queue.
        Node *node = remove_head_linked_list(queue);

        int burstTimeTemp = node->process->burstTime;
        burstTimeTemp -= TIME_QUANTUM;

        // Waiting time is equal to overall time elapsed minus the time current process spent executing.
        node->process->waitingTime = timeElapsed - node->process->timeSpentProcessing;

        // Negative burst time means process didn't meet time quantum.
        if(burstTimeTemp >= 0) {
          timeElapsed += TIME_QUANTUM;
          node->process->timeSpentProcessing += TIME_QUANTUM;
          // Update process burst time by subtracting the time it has been executing.
          node->process->burstTime -= TIME_QUANTUM; 
        } else {
          timeElapsed += abs(TIME_QUANTUM + burstTimeTemp);
          node->process->timeSpentProcessing += abs(TIME_QUANTUM + burstTimeTemp);

        }


        // Remove process, if it finished executing.
        if(burstTimeTemp <= 0) {
            node->process->completionTime = timeElapsed - node->process->arrivalTime;

            printProcessInfo(node->process);

            free(node);
            continue;
        } 


        // Otherwise, put process to the end of the queue.
        append_linked_list(queue, node->process);
    }

}

/* Prints all the information about the process. */
void printProcessInfo(Process *process) {
    printf("\n__________________________________________\n");
    printf("Process (%d) has finshed executing\n\n", process->pId);
    printf("    - Completion time: (%d)\n", process->completionTime);
    printf("    - Turn-around time: (%d)\n", process->turnAroundTime);
    printf("    - Waiting time: (%d)\n", process->waitingTime);
    printf("    - Burst time: (%d)\n", process->burstTime);
    printf("    - Time Spent Processng: (%d)\n", process->timeSpentProcessing);
    printf("__________________________________________\n");
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
  
int main(int argc, char *argv[]) {

  	// Allocate memory for process queue.
  	LinkedList *processQueue = initialise_linked_list();

    // If not even number of inputs.
    if((argc - 1) % 2 != 0) {
        printf("(ERROR) Invalid parameters.\n");
        printf("Should be in the format of { (Burst time 1) (Arrival time 1) (Burst time 2) (Arrival time 2) ... }\n");
        printf("For example: RR 54 32 2 32 12 2\n");
        return 1;
    }

    // check all int 
    for(int i = 1; i < argc; i++) {
      if(!isInt(argv[i])) {
        printf("(ERROR) All inputs need to be integers");
        return 1;
      }
    }
    
    // Append all inputs to linked list.
    for(int i = 1; i < argc; i += 2) {
        char *ptr;
        int burstTime = strtol(argv[i], &ptr, 10);
        int arrivalTime = strtol(argv[i + 1], &ptr, 10);
        
        append_linked_list(processQueue, newProcess(burstTime, arrivalTime));
    }



    roundRobin(processQueue);
    
    // free process queue memory
    free_linked_list(processQueue);

}
