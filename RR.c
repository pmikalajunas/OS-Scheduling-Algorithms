#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 

// -------------------- CONSTANTS -------------------- 
int const TIME_QUANTUM = 20; // Fixed time quantum.
int const INCREASE_FACTOR = 2; // Factor by which queue length increases.
// --------------------------------------------------- 

// -------------------- VARIABLES -------------------- 

int queueLength = 64; // May be resized, if exceded.
int processCount = 0; // Incremented with each input
// --------------------------------------------------- 


// -------------------- STRUCTURES -------------------- 
typedef struct Process
{
    int pId;
    int burstTime;
    int arrivalTime;
    int completionTime;
    int turnAroundTime;
    int waitingTime;
    
} Process;

typedef struct ComparisonData
{
    int averageTurnaroundTime;
    int averageWaitingTime;
    int throughput;
} ComparisonData;
// --------------------------------------------------- 


// -------------------- LINKED LIST --------------------

typedef struct Node {
  Process *process;
  struct Node *prev;
  struct Node *next;
} Node;

typedef struct LinkedList {
  Node *head;
  Node *tail;
} LinkedList;

/* create a new node */
/* returns a pointer to the newly created node */
/* print an error message and return NULL if an error occurs */
Node *initialise_node(void)
{
    Node *newNode = (Node*) malloc(sizeof(Node));
    if(!newNode){
        fprintf(stderr, "(initialise_node) Malloc failed at line: (%d)\n", __LINE__);
        return NULL;
    }

    //newNode->process = NULL;
    newNode->next = NULL;
    newNode->prev = NULL;

    return newNode;
}

/* free memory for node *node */
/* print an error message and return if node is NULL */
void free_node(Node *node)
{
    if(!node){
        fprintf(stderr, "(free_node) Node was NULL at line: (%d)\n", __LINE__);
        return;
    }

    free(node);
}

/* create a new linked list */
/* returns a pointer to the newly created list */
/* print an error message and return NULL if an error occurs */
LinkedList *initialise_linked_list(void)
{
    LinkedList *newList = (LinkedList*) malloc(sizeof(LinkedList));

    if(!newList){
        fprintf(stderr, "(initialise_linked_list) Malloc failed at line: (%d)\n", __LINE__);
        return NULL;
    }

    newList->head = NULL;
    newList->tail = NULL;
    return newList;
}

/* create and add node to the tail of linked list *list */
/* and set data stored at node to *data */
/* should return a pointer to the new node */
/* should return NULL if an error occurs */
Node *append_linked_list(LinkedList *list, Process *process) 
{  
    Node *node;

	  node = initialise_node();

	  node->process = process;
	  node->prev = list->tail;
	  if(list->tail) {
		  list->tail->next = node;
	  }
	  list->tail = node;
	  if(!list->head)
		  list->head = node;

}

/* remove head from linked list *list */
/* print an error message and return if list is NULL or empty */
void remove_head_linked_list(LinkedList *list)
{
    Node *oldNode;
    if(!list) {
        fprintf(stderr, "Error: Attempting to remove head from null linked list.\n");
        return;
    }
    if(!list->head) {
        fprintf(stderr, "Error: Attempting to remove head from empty linked list.\n");
        return;
    }
    if(!list->head->next) { /* if only 1 elem */
        free(list->head);
    } else {
        oldNode = list->head;
        list->head = oldNode->next;
        list->head->prev = NULL;
        free(oldNode);
    }

}


/* free memory for linked list *list */
/* frees memory for all nodes in linked list and list itself */
/* print an error message and return if list is NULL */
void free_linked_list(LinkedList *list)
{
    /* is list NULL??? */
    if(!list){
        fprintf(stderr, "(free_linked_list) List was NULL at line: (%d)\n", __LINE__);
        return;
    }

    /* freeing each node */
    while(list->head){
        Node *temp = list->head;
        list->head = list->head->next;
        /* freeing the node */
        free_node(temp);
    }

    /* ant the list itself... */
    free(list);
}

// -------------------- FUNCTIONS --------------------



void roundRobin(Process *queue) {

    while(1) {
        
    }

}


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
    
    //for(int i = 0 ; i < argc; i++) printf("[(%i) - %s]",i, argv[i]);

    
    for(int i = 1; i < argc; i += 2) {
        char *ptr;
        int burstTime = strtol(argv[i], &ptr, 10);
        int arrivalTime = strtol(argv[i + 1], &ptr, 10);
        
        
        append_linked_list(processQueue, newProcess(burstTime, arrivalTime));
    }

    printf("(%d)", processQueue->head->next->process->arrivalTime);
    
/*
    while(1) {
        printf("Enter process %d's burst time: ", processCount);
      
      	int burst = getInt();
      
      	if(burst < 0) break;
      	
        if(processCount < queueLength) {
            processQueue[processCount++] = *newProcess(burst);
        } else {
            // Allocate memory for extra processes.
            queueLength *= INCREASE_FACTOR;
            processQueue = (Process*) realloc(processQueue, sizeof(Process) * queueLength);
            memCheck(processQueue);
            processQueue[processCount++] = *newProcess(burst);
        }
    }*/
  
  	// Free the memory which is not used.
    //if(processCount < queueLength) {
        //processQueue = (Process*) realloc(processQueue, sizeof(Process) * processCount);
      //  memCheck(processQueue);
    //}
  
  
  	// Free the allocated memory.
    // free(processQueue);
    free_linked_list(processQueue);

}

