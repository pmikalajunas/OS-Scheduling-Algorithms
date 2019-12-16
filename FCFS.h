
#ifndef SCHEDULER_HELPER 
#define SCHEDULER_HELPER
    #include "./SchedulerHelper.h"
#endif


/**
 * Implementation of First Come First Served process scheduling algorithm.
 * Method simulates CPU clock ticks, one iteration of a loop is one clock tick or 1ms
 * 
 * @pararm processQueue - is the queue containing processes that are ready to run.
 * @param waitingQueue - is the queue containing processes with arrival time.
 * @param NOTUSED - is left there for further implementation of additional queues for MLFQ,
 * this argument allows us to call both scheduling methods as arguments
 * @return Method returns a a list of processes in the order that they were completed.
 * */
LinkedList* firstComeFirstServed(LinkedList *processQueue, LinkedList *waitingQueue, LinkedList *NOTUSED){


    // Defines how much time we spent executing the current process.
    int timeSpentOnIteration = 0;
    LinkedList *completedQueue = initialise_linked_list();

    while(!linked_list_empty(processQueue) || !linked_list_empty(waitingQueue)){

        // Adding node from waiting queue, before taking one from processQueue.
        addWaitingNode(waitingQueue, processQueue);

        // Getting the head node of the queue.
        Node *node = remove_head_linked_list(processQueue);

        // If processing queue is empty, we keep on going with another CPU cycle.
        if(!node) {           
            printEmptyQueueError();
            timeElapsed++;
            continue;
        }

        printProcessingHeader(node);


        // Retrieving remaining time considering the current iteration.
        int remainingTime = node->process->remainingTime - timeSpentOnIteration;

        // We keep running through CPU cycles while process is being processed.
        // We check if there are nodes in the waiting queue.
        while(remainingTime) {
            timeSpentOnIteration++;
            timeElapsed++;
            remainingTime = node->process->remainingTime - timeSpentOnIteration;

            if(node->process->remainingTime == node->process->burstTime) {
                node->process->responseTime = timeElapsed - node->process->arrivalTime;
            }


            printTimeInfo(timeElapsed, timeSpentOnIteration, remainingTime);



            addWaitingNode(waitingQueue, processQueue);
        }
        printLine();
        

        node->process->timeSpentProcessing = node->process->burstTime;
        discardProcess(node, completedQueue);
        // Resetting time spent on iteration, next process will take turn.
        timeSpentOnIteration = 0;

    }

    return completedQueue;
}