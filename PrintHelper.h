



/**
 * This header files contains all the prints functions uses throughout the software.
 * Most of them are used for debugging.
 * */


ComparisonData *collectComparisonData(LinkedList *completedQueue) {

    ComparisonData *data = (ComparisonData *) malloc(sizeof(ComparisonData));
    if(data == NULL) {
        printf("(printComparisonData) failed to initialise memory for new node!\n");
        return NULL;
    }
    
    Node *curProcess = peek_head_linked_list(completedQueue);

    if(curProcess == NULL) {
        printf("(printComparisonData) passed queue was empty!\n");
        return NULL;
    }

    float totalNodes = 0,
          totalTurnaroundTime = 0,
          totalWaitingTime = 0,
          totalCompletionTime = 0,
          totalBurstTime = 0,
          totalArrivalTime = 0,
          totalResponseTime = 0;

    do {
        totalNodes++;
        totalTurnaroundTime += curProcess->process->turnAroundTime;
        totalWaitingTime += curProcess->process->waitingTime;
        totalCompletionTime += curProcess->process->completionTime;
        totalBurstTime += curProcess->process->burstTime;
        totalArrivalTime += curProcess->process->arrivalTime;
        totalResponseTime += curProcess->process->responseTime;
        
    } while((curProcess = curProcess->next));

    data->averageTurnaroundTime = totalTurnaroundTime / totalNodes;
    data->averageWaitingTime = totalWaitingTime / totalNodes;
    data->averageCompletionTime = totalCompletionTime / totalNodes;
    data->averageBurstTime = totalBurstTime / totalNodes;
    data->averageArrivalTime = totalArrivalTime / totalNodes;
    data->averageResponseTime = totalResponseTime / totalNodes;
    data->throughput = totalNodes / timeElapsed;

    return data;
}

void printComparisonData(LinkedList *completedQueue) {

    ComparisonData *data = collectComparisonData(completedQueue);
    if(data == NULL) {
        return;
    }

    printf("\n+-------------------------------------------------------------------------------------+\n");
    printf("|                                  Statistics                                         |\n");
    printf("+-----+-----------------+--------------+------------------+------------+--------------+\n");
    printf(" Time Elapsed = ( %d )\n", timeElapsed);
    printf(" Average Turnaround Time = ( %f )\n", data->averageTurnaroundTime);
    printf(" Average Waiting Time = ( %f )\n", data->averageWaitingTime);
    printf(" Average Completion Time = ( %f )\n", data->averageCompletionTime);
    printf(" Average Burst Time = ( %f )\n", data->averageBurstTime);
    printf(" Average Arrival Time = ( %f )\n", data->averageArrivalTime);
    printf(" Average Response Time = ( %f )\n" , data->averageResponseTime);
    printf(" Throughput = ( %f )\n", data->throughput);
    printf("+-----+-----------------+--------------+------------------+------------+--------------+\n");

    free(data);
}


/**
 * Prints information about the constraints that we have set.
 * */
void printConstraintInfo() {
        printf("____________________________________________________________________________\n");
    printf("(ERROR) Invalid parameters.\n");
    printf("Burst time should be a number between 1 and %d\n", MAX_BURST_TIME);
    printf("Arrival time should be a number between 0 and %d\n", MAX_ARRIVAL_TIME);
    printf("You should not be able to enter more than %d processes!\n", PROCESS_LIMIT);
    printf("____________________________________________________________________________\n");
}


/**
 * Prints information about the node which was added from the waiting queue.
 * */
void printAddedNodeInfo(Node *addedNode) {
    if(!DEBUG) return;
    printf("\n+++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("\nNode (ID: %d) is added from the waiting queue\n",
            addedNode->process->pId);   
    printf("Time elapsed: %d\n", addedNode->process->arrivalTime);
    printf("\n+++++++++++++++++++++++++++++++++++++++++++++++\n");
}

/**
 *  Prints all the information about the process once it finishes executing.
 * */
void printProcessInfo(Process *process) {
    if(!DEBUG) return;
    printf("    - Completion time: (%d)\n", process->completionTime);
    printf("    - Turn-around time: (%d)\n", process->turnAroundTime);
    printf("    - Waiting time: (%d)\n", process->waitingTime);
    printf("    - Burst time: (%d)\n", process->burstTime);
    printf("    - Arrival time: (%d)\n", process->arrivalTime);
    printf("    - Time Spent Processing: (%d)\n", process->timeSpentProcessing);
    printf("    - Remaining burst time: (%d)\n", process->remainingTime);
}


/**
 * Prints information on how to use the program, if user uses wrong CMD arguments. 
 * */
void printGuidelines() {
    printf("____________________________________________________________________________\n");
    printf("(ERROR) Invalid parameters.\n");
    printf("Should be in the format of { (Burst time 1) (Arrival time 1) (Burst time 2) (Arrival time 2) ... }\n");
    printf("For example: RR 54 32 2 32 12 2\n");
    printf("Or FCFS 54 32 2 32 12 2\n");
    printf("____________________________________________________________________________\n");
}


/**
 * Prints detailed about every process that has been executed.
 * Processes are ordered the way they have been terminated.
 * */
void printProcessTable(LinkedList *completedQueue) {

    printf("\n+-------------------------------------------------------------------------------------+\n");
    printf("|                                  Executed Processes                                 |\n");
    printf("+-----+-----------------+--------------+------------------+------------+--------------+\n");
    printf("|  ID | Completion Time | Waiting Time | Turn-Around Time | Burst Time | Arrival Time |\n");

    // The following line will be used to separate processes from each other.
    char line[] = "+-----+-----------------+--------------+------------------+------------+--------------+\n";
    printf("%s", line);

    Node *node = peek_head_linked_list(completedQueue);

    do {
        printf("|%5d|%17d|%14d|%18d|%12d|%14d|\n",
                node->process->pId, node->process->completionTime,
                node->process->waitingTime, node->process->turnAroundTime,
                node->process->burstTime, node->process->arrivalTime);
        printf("%s", line);
    } while((node = node->next)); 

}


/**
 * Prints process ID and time elapsed.
 * Used in both RR and FCFS to print information on each iteration.
 * */
void printProcessingHeader(Node *node) {
    if(!DEBUG) return;
    printf("\ntimeElapsed: (%d)", timeElapsed);
    printf("\n____________________________________________________________________________\n");
    printf("\nProcess (ID: %d) is being processed\n", node->process->pId);
}


/**
 * Informs about the empty processing queue, prints out the elapsed time.
 * */
void printEmptyQueueError() {
    if(!DEBUG) return;
    printf("\n__________________________________________________________\n");
    printf("Processing queue is empty, proceeding with another cycle.\n");
    printf("timeElapsed: (%d)\n", timeElapsed);
    printf("\n__________________________________________________________\n");
}


void printTimeInfo(int timeElapsed, int timeSpentOnIteration, int remainingTime) {
    if(!DEBUG) return;
    printf("\ntimeElapsed: (%d), timeSpentOnIteration: (%d), remainingTime: (%d)\n",
            timeElapsed, timeSpentOnIteration, remainingTime);
}

void printLine() {
    if(!DEBUG) return;
    printf("\n____________________________________________________________________________\n"); 
}

void printRoundRobinFinishedExecuting() {
    if(!DEBUG) return;
    printf("\n_________________________________________________________\n");
    printf("Round Robin finished executing\n");
    printf("\n_________________________________________________________\n");
}