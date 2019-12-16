
/**
 * This header files includes all the input reading functions
 * Includes input validation functions.
 * */



// Check if input string contains all integers.
// Return true if all digits, false if not.
bool isInt(char* input) {
  for(int i = 0; i < strlen(input); i++) {
    if(!isdigit(input[i])) {
      return false;
    }
  }
  return true;
}


// Allocate memory to new process and initialise it.
Process *newProcess(int burstTime, int arrivalTime) {

    // Allocating memory for a process, if available.
    Process *p = (Process *) malloc(sizeof(Process));
    if(p == NULL) {
        return NULL;
    }

    // Set the burst time, pId and return the value.
    p->pId = processCount++;
    p->burstTime = burstTime;
    p->arrivalTime = arrivalTime;
    p->remainingTime = burstTime;
    p->completionTime = 0;
    p->turnAroundTime = 0;
    p->waitingTime = 0;
    p->timeSpentProcessing = 0;

    return p;
}

// Ensure all inputs to the program are integers.
// Checks if burst time is more or equal to 1.
// Checks if both burst time and arrival time meets the constraints.
bool verifyAllInputsInt(int argc,  char **argv) {

    for(int i = 1; i < argc; i++) {
      if(!isInt(argv[i])) {
        printf("(ERROR) All inputs has to be positive integers!\n");
        return false;
      }
      int x = atoi(argv[i]);
      if((i % 2 != 0) && (x < 1 || x > MAX_BURST_TIME)) {
          printf("(ERROR) Burst has to be a number between 1 and %d!\n", MAX_BURST_TIME);
          return false;
      } else if (x < 0 || x > MAX_ARRIVAL_TIME) {
          printf("(ERROR) Arrival time has to be a number between 0 and %d!\n", MAX_ARRIVAL_TIME);
          return false;
      }
    }
    return true;

}

/**
 * Reads a list of processes with their burst and arrival times from CMD.
 * In the format: (Burst time 1) (Arrival time 1) (Burst time 2) (Arrival time 2) ...
 * Process with arrival time are added to the waiting queue, without - to the processQueue.
 * */
void readCommandLineArguments(int argc, char *argv[], LinkedList *processQueue, LinkedList *waitingQueue) {

    for(int i = 1; i < argc; i += 2) {
        char *ptr;
        int burstTime = (int) strtol(argv[i], &ptr, 10);
        int arrivalTime = (int) strtol(argv[i + 1], &ptr, 10);

        if(arrivalTime == 0) {
            append_linked_list(processQueue, newProcess(burstTime, arrivalTime));
        } else {
            append_linked_list(waitingQueue, newProcess(burstTime, arrivalTime));
        }
    }
}



/** 
 * Checks if user passed a right amount of processes.
 * @return Returns False if it doens't.
 * Returns True if it does.
 * */
bool isCorrectNumberOfProcesses(int argc) {
    return (((argc - 1) / 2) <= PROCESS_LIMIT);
}

/**
 * Tells if user passed an equal number of arguments.
 * It's important because we have to have arrival ...
 * ... and burst time for every process.
 * */
bool isEvenNumberOfArguments(int argc) {
    return (argc - 1) % 2 != 0;
}




/**
 * Validates input by checking if we have right amount of arguments.
 * Checks if every character is an integer.
 * Checks if there is any input at all!
 * Checks if input meets our set constraints.
 * Returns INPUT_ERROR if input is user input is not suitable.
 * Returns 0 if input is correct.
 * */
int validateInput(int argc, char *argv[]) {

    // No arguments.
    if(argc <= 1) {
        printGuidelines();
        return INPUT_ERROR;
    }

    if(!isCorrectNumberOfProcesses(argc)) {
        printConstraintInfo();
        return INPUT_ERROR;
    }


    if(isEvenNumberOfArguments(argc)) {
        printGuidelines();
        return INPUT_ERROR;
    }
    if(!verifyAllInputsInt(argc,  argv)) return INPUT_ERROR;

    return SUCCESSFUL_EXECUTION;
}

