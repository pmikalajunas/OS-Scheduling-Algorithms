#ifndef SCHEDULER_HELPER 
#define SCHEDULER_HELPER
    #include "./SchedulerHelper.h"
#endif



/**
 * RoundRobin scheduling algorithm implementation.
 * Returns a queue of completed processes, in their order of completion.
 * Simulates CPU clock cycle, assumes that one CPU clock cycle is one milisecond.
 * Each iteration of the loop is one clock cycle.
 * Takes list of of processes in processQueue, without the arrival time.
 * Takes a list of processes in waitingQueue, with arrival time.
 * */
LinkedList* roundRobin(LinkedList *processQueue, LinkedList *waitingQueue, LinkedList *appendingQueue) {


    // Defines how much time we spent executing the current process.
    int timeSpentOnIteration = 0;

    // A list of processes which have finished executing.
    LinkedList *completedQueue = initialise_linked_list();

    // We have to keep the algorithm until we have processes left in either process or waiting queue.
    while(!linked_list_empty(processQueue) || !linked_list_empty(waitingQueue)) {
        

        // Adding node from waiting queue, before taking one from processQueue.
        addWaitingNode(waitingQueue, processQueue);

        // Getting the head node of the queue.
        Node *node = remove_head_linked_list(processQueue);


        // If processing queue is empty, we keep on going with another CPU cycle.
        if(!node) {     
            timeElapsed++;      
            printEmptyQueueError();            
            continue;
        }

        printProcessingHeader(node);

        // Retrieving remaining time considering the current iteration.
        int remainingTime = node->process->remainingTime - timeSpentOnIteration;

        // We keep running through CPU cycles while process is being processed.
        // We check if there are nodes in the waiting queue and we make sure TIME_QUANTUM is not exeeded.
        while(remainingTime && (TIME_QUANTUM > timeSpentOnIteration)) {
            timeSpentOnIteration++;
            timeElapsed++;
            remainingTime = node->process->remainingTime - timeSpentOnIteration;
            printf("\ntimeElapsed: (%d), timeSpentOnIteration: (%d), remainingTime: (%d)\n",
             timeElapsed, timeSpentOnIteration, remainingTime);

            addWaitingNode(waitingQueue, processQueue);
        }
        printf("\n____________________________________________________________________________\n"); 
            

        node->process->timeSpentProcessing += timeSpentOnIteration;

        // Updating process' remaining time by subtracrting the time it spent executing.
        // (Initially remaining time is set to be equal to burst time.)
        node->process->remainingTime -= timeSpentOnIteration;

        // If process is not done yet add it back to the processing queue.
        if(node->process->remainingTime > 0) {
            append_linked_list(appendingQueue, node->process);
            printf("Process (ID: %d) appended back to the queue.\n", node->process->pId);
            printProcessInfo(node->process);
        } else {
            discardProcess(node, completedQueue);
        }

        // Resetting time spent on iteration, next process will take turn.
        timeSpentOnIteration = 0;
 
    }
    return completedQueue;
}