// Multi level feedback queue
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "./LinkedList.h"
#include "./SchedulerHelper.h"


// timeElapsed is a simulation of CPU cycles, every iteration is one CPU clock cycle
// 1 CPU Clock Cycle = 1ms
 int timeElapsed = 0;
// Defines how much time we spent executing the current process.
int timeSpentOnIteration = 0;

LinkedList* roundRobin(LinkedList *processQueue, LinkedList *waitingQueue, LinkedList *completedQueue);
LinkedList* firstComeFirstServe(LinkedList *processQueue, LinkedList *waitingQueue, LinkedList *completedQueue);
LinkedList *FCFSQueue, *RRQueue, *FCFSCompletedQueue, *RRCompletedQueue, *waitingQueue, *completedQueue;

int main() {
    RRQueue = initialise_linked_list();
    FCFSQueue = initialise_linked_list();
    waitingQueue = initialise_linked_list();
    
    // A list of processes which have finished executing.
    completedQueue = initialise_linked_list();

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

  //  if burst time > time quantum then burst time - time quantum goes to FCFS queue.


    roundRobin(RRQueue, waitingQueue, completedQueue);
    firstComeFirstServe(FCFSQueue, waitingQueue, completedQueue);
    
    
    printProcessTable(completedQueue);

}





/**
 * RoundRobin scheduling algorithm implementation.
 * Returns a queue of completed processes, in their order of completion.
 * Simulates CPU clock cycle, assumes that one CPU clock cycle is one milisecond.
 * Each iteration of the loop is one clock cycle.
 * Takes list of of processes in processQueue, without the arrival time.
 * Takes a list of processes in waitingQueue, with arrival time.
 * */
LinkedList* roundRobin(LinkedList *processQueue, LinkedList *waitingQueue, LinkedList *completedQueue) {

    // We have to keep the algorithm until we have processes left in either process or waiting queue.
    while(!linked_list_empty(processQueue) || !linked_list_empty(waitingQueue)) {

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
        // We check if there are nodes in the waiting queue and we make sure TIME_QUANTUM is not exeeded.
        while(remainingTime && (TIME_QUANTUM > timeSpentOnIteration)) {
            timeSpentOnIteration++;
            timeElapsed++;
            remainingTime = node->process->remainingTime - timeSpentOnIteration;
            printf("\ntimeElapsed: (%d), timeSpentOnIteration: (%d), remainingTime: (%d)\n",
             timeElapsed, timeSpentOnIteration, remainingTime);

            addWaitingNode(waitingQueue, processQueue, timeElapsed);
        }
        printf("\n____________________________________________________________________________\n"); 
            

        node->process->timeSpentProcessing += timeSpentOnIteration;

        // Updating process' remaining time by subtracrting the time it spent executing.
        // (Initially remaining time is set to be equal to burst time.)
        node->process->remainingTime -= timeSpentOnIteration;

        // If process is not done yet add it back to the processing queue.
        if(node->process->remainingTime > 0) {
            append_linked_list(FCFSQueue, node->process);
            printf("Process (ID: %d) appended back to the queue.\n", node->process->pId);
            printProcessInfo(node->process);
        } else {
            discardProcess(node, timeElapsed, completedQueue);
        }

        // Resetting time spent on iteration, next process will take turn.
        timeSpentOnIteration = 0;
 
    }
    return completedQueue;
}




LinkedList* firstComeFirstServe(LinkedList *processQueue, LinkedList *waitingQueue, LinkedList *completedQueue){

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




