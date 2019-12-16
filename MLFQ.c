
#include "./RR.h"
#include "./FCFS.h"

int main(int argc, char *argv[]) {

    if(validateInput(argc, argv) == INPUT_ERROR) {
        return INPUT_ERROR;
    }


    LinkedList *RRQueue = initialise_linked_list();
    LinkedList *FCFSQueue = initialise_linked_list();
    LinkedList *waitingQueue = initialise_linked_list();
    
    // A list of processes which have finished executing.
    LinkedList *completedQueueRR = initialise_linked_list();
    LinkedList *completedQueueFCFS = initialise_linked_list();
   

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
    printf("\n_________________________________________________________\n");
    printf("Round Robin finished executing\n");
    printf("\n_________________________________________________________\n");
    completedQueueFCFS = firstComeFirstServed(FCFSQueue, waitingQueue, NULL);
    
    
    LinkedList *completedQueue = merge_linked_lists(completedQueueRR, completedQueueFCFS);

    printProcessTable(completedQueue);

    free_linked_list(completedQueue);

}

