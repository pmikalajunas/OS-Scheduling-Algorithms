// Multi level feedback queue

#include <stdio.h>
#include "./LinkedList.h"
#include "./SchedulerHelper.h"

void main(int argc, char **argv) {
    // Priority queue, RR.
    LinkedList *RRQueue, *FCFSQueue, *rrCompletedQueue, *fcfsCompletedQueue waitingQueue;
    
    RRQueue = initialise_linked_list();
    FCFSQueue = initialise_linked_list();
    completedQueue = initialise_linked_list();
    waitingQueue = initialise_linked_list();

    
    if(isEvenNumberOfArguments(argc)) {
        printGuidelines();
        return 1;
    }
    
    // Ensure all inputs are integers
    if(!verifyAllInputsInt(argc,  argv)) return 1;

    // Process goes to RR queue

    // if burst time > time quantum then burst time - time quantum goes to FCFS queue.


    rrCompletedQueue = roundRobin(RRQueue, waitingQueue, FCFSQueue);
    fcfsCompletedQueue = firstComeFirstServed(FCFSQueue);
    
    merge_linked_lists(rrCompletedQueue, fcfsCompletedQueue);

    printProcessTable(rrCompletedQueue);
    
}



// TODO: we might simulate actual CPU clock ticks
LinkedList* roundRobin(LinkedList *processQueue, LinkedList *waitingQueue, LinkedList *FCFSQueue) {

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

            // Update process burst time by subtracting the time it has been executing.
            node->process->burstTime -= TIME_QUANTUM;

            printf("Process (ID: %d) left CPU (is not finished), timeElapsed: (%d)\n",
                    node->process->pId, timeElapsed);           
            printProcessInfo(node->process);

             // Process is put at the end of the processing queue.
            append_linked_list(FCFSQueue, node->process);
        } else {
            int executionTime = abs(TIME_QUANTUM + burstTimeTemp);
            timeElapsed += executionTime;

            // Add the node from the waiting queue (if it's ready).
            addWaitingNode(waitingQueue, processQueue, timeElapsed);

            discardProcess(node, timeElapsed, completedQueue);
        }


        printf("\n____________________________________________________________________________\n");
    }
    return completedQueue;
}




LinkedList* firstComeFirstServe(LinkedList *FCFSQueue){

    int timeElapsed = 0;
    int count = 0;

    // A list of processes which have finished executing.
    LinkedList *completedQueue = initialise_linked_list();

    while(!linked_list_empty(processQueue) && !linked_list_empty()){

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
        // waiting  + burst
        node->process->waitingTime = timeElapsed - node->process->burstTime;
      // completeion  - arrival 
        printf("Process (ID: %d) has finished, timeElapsed: (%d)\n",
              node->process->pId, timeElapsed);
        printProcessInfo(node->process);

        append_linked_list(completedQueue, node->process);

        free(node);

        printf("\n____________________________________________________________________________\n");
    }

    return completedQueue;
}
