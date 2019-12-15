
#ifndef SCHEDULER_HELPER 
#define SCHEDULER_HELPER
    #include "./SchedulerHelper.h"
#endif




LinkedList* firstComeFirstServe(LinkedList *processQueue, LinkedList *waitingQueue){

    // timeElapsed is a simulation of CPU cycles, every iteration is one CPU clock cycle
    // 1 CPU Clock Cycle = 1ms
    int timeElapsed = 0;
    // Defines how much time we spent executing the current process.
    int timeSpentOnIteration = 0;

    // A list of processes which have finished executing.
    LinkedList *completedQueue = initialise_linked_list();

    while(!linked_list_empty(processQueue) || !linked_list_empty(waitingQueue)){

        // Adding node from waiting queue, before taking one from processQueue.
        addWaitingNode(waitingQueue, processQueue, timeElapsed);

        // Getting the head node of the queue.
        Node *node = remove_head_linked_list(processQueue);

        // If processing queue is empty, we keep on going with another CPU cycle.
        if(!node) {           
            printEmptyQueueError(timeElapsed++);
            continue;
        }

        printProcessingHeader(timeElapsed, node);


        // Retrieving remaining time considering the current iteration.
        int remainingTime = node->process->remainingTime - timeSpentOnIteration;

        // We keep running through CPU cycles while process is being processed.
        // We check if there are nodes in the waiting queue.
        while(remainingTime) {
            timeSpentOnIteration++;
            timeElapsed++;
            remainingTime = node->process->remainingTime - timeSpentOnIteration;
            printf("\ntimeElapsed: (%d), timeSpentOnIteration: (%d), remainingTime: (%d)\n",
             timeElapsed, timeSpentOnIteration, remainingTime);

            addWaitingNode(waitingQueue, processQueue, timeElapsed);
        }
        printf("\n____________________________________________________________________________\n"); 


 

        node->process->timeSpentProcessing = node->process->burstTime;
        discardProcess(node, timeElapsed, completedQueue);
        // Resetting time spent on iteration, next process will take turn.
        timeSpentOnIteration = 0;

    }

    return completedQueue;
}