#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> 
#include <ctype.h>
#include "./LinkedList.h"

// -------------------- CONSTANTS -------------------- 
int const TIME_QUANTUM = 20; // Fixed time quantum.
int const INCREASE_FACTOR = 2; // Factor by which queue length increases.
// --------------------------------------------------- 

// -------------------- VARIABLES -------------------- 
int queueLength = 64; // May be resized, if exceded.
int processCount = 0; // Incremented with each input
// --------------------------------------------------- 

// -------------- FUNCTION DEFINITIONS ---------------
bool isInt(char*);

// -------------------- FUNCTIONS --------------------



void roundRobin(Process *queue) {

    // While process queue is not empty.
    while(1) {
        // Remove process, if it finished executing.
        if(remove_head_linked_list) {

        }

        
        
    }

}

// Allocate memory to new process and initialise it.
Process *newProcess(int burstTime, int arrivalTime) {

    // Allocating memory for a process, if available.
    Process *p = malloc(sizeof(Process));
    if(p == NULL) {
        return NULL;
    }

    // Set the burst time, pId and return the value.
    p->pId = processCount;
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
        printf("Should be in the format of { (Arrival time 1) (Burst time 1) (Arrival time 2) (Burst time 2) ... }\n");
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
    
    // free process queue memory
    free_linked_list(processQueue);

}
