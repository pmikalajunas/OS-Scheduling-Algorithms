
#include "./RR.h"
#include "./FCFS.h"


// timeElapsed is a simulation of CPU cycles, every iteration is one CPU clock cycle
// 1 CPU Clock Cycle = 1ms
 int timeElapsed = 0;
// Defines how much time we spent executing the current process.
int timeSpentOnIteration = 0;


int main() {

    LinkedList *RRQueue = initialise_linked_list();
    LinkedList *FCFSQueue = initialise_linked_list();
    LinkedList *waitingQueue = initialise_linked_list();
    
    // A list of processes which have finished executing.
    LinkedList *completedQueue = initialise_linked_list();
    LinkedList *completedQueue2 = initialise_linked_list();


    int argc = 5;
    char *argv[] = {"MLFQ", "5", "0", "6", "0"};
    


    if(isEvenNumberOfArguments(argc)) {
        printGuidelines();
        return 1;
    }
    
    // Ensure all inputs are integers
    if(!verifyAllInputsInt(argc,  argv)) return 1;

   

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


    completedQueue = roundRobin(RRQueue, waitingQueue, FCFSQueue);
    completedQueue2 = firstComeFirstServe(FCFSQueue, waitingQueue);
    
    
    merge_linked_lists(completedQueue, completedQueue2);

    printProcessTable(completedQueue);

}

