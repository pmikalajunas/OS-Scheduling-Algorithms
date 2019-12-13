#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> 
#include <math.h>
#include <ctype.h>
#include "./LinkedList.h"

// -------------------- VARIABLES -------------------- 
int queueLength = 64; // May be resized, if exceded.
int processCount = 0; // Incremented with each input
// --------------------------------------------------- 

// -------------- FUNCTION DEFINITIONS ---------------
void firstComeFirstServe(LinkedList *queue);
void printProcessInfo(Process *process);


// -------------------- FUNCTIONS --------------------

void firstComeFirstServe(LinkedList *queue){
    int timeElapsed = 0;

    while(!linked_list_empty(queue)){
        Node *node = remove_head_linked_list(queue);
        
        node->process->waitingTime = timeElapsed;
        timeElapsed += node->process->burstTime;

        node->process->completionTime = timeElapsed - node->process->arrivalTime;
        node->process->turnAroundTime = node->process->waitingTime + node->process->burstTime;
        printProcessInfo(node->process);

        free(node);
        continue;
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

int main(){

    int argc = 7    ;
    char *argv[] = {"FCFS", "54", "32", "2", "32", "12", "2"};
    
    // Allocate memory for process queue.
    LinkedList *processQueue = initialise_linked_list();

    // If not even number of inputs.
    if((argc - 1) % 2 != 0) {
        printf("(ERROR) Invalid parameters.\n");
        printf("Should be in the format of { (Burst time 1) (Arrival time 1) (Burst time 2) (Arrival time 2) ... }\n");
        printf("For example: FCFS 54 32 2 32 12 2\n");
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

    firstComeFirstServe(processQueue);

    // free process queue memory
    free_linked_list(processQueue);
}