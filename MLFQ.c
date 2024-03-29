
#include "./RR.h"
#include "./FCFS.h"

/**
 * Implementation of multi level queue scheduling algorithm.
 * First queue is served by RR algorithm.
 * Second queue is serverd by FCFS algorithm.
 * When process completes executing (but it's done) it gets placed on 2nd queue.
 * Once we finish executing first queue, we move on to the second queue.
 * */
int main(int argc, char *argv[]) {

    if(validateInput(argc, argv) == INPUT_ERROR) {
        return INPUT_ERROR;
    }


    LinkedList *RRQueue = initialise_linked_list();
    LinkedList *FCFSQueue = initialise_linked_list();
    LinkedList *waitingQueue = initialise_linked_list();
    
    // A list of processes which have finished executing.
    LinkedList *completedQueueRR;
    LinkedList *completedQueueFCFS;
   

    // Process goes to RR queue
    for(int i = 1; i < argc; i += 2) {
        char *ptr;
        int burstTime = (int) strtol(argv[i], &ptr, 10);
        int arrivalTime = (int) strtol(argv[i + 1], &ptr, 10);

        if(arrivalTime == 0) {
            append_linked_list(RRQueue, newProcess(burstTime, arrivalTime));
        } else {
            append_linked_list(waitingQueue, newProcess(burstTime, arrivalTime));
        }

    }

    // Sort the list of processes by their arrival time.
    // Allows us to just check the head each time.
    bubbleSort(waitingQueue->head);

    completedQueueRR = roundRobin(RRQueue, waitingQueue, FCFSQueue);
    printRoundRobinFinishedExecuting();

    completedQueueFCFS = firstComeFirstServed(FCFSQueue, waitingQueue, NULL);
    
    
    LinkedList *completedQueue = merge_linked_lists(completedQueueRR, completedQueueFCFS);


    printProcessTable(completedQueue);
    printComparisonData(completedQueue);

    free_linked_list(completedQueue, true);
    free_linked_list(waitingQueue, true);
    free_linked_list(RRQueue, true);
    free_linked_list(FCFSQueue, true);


}

