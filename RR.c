
#include "./RR.h"


/**
 * Main fuctions takes a list of processes with their arrival and burst times from the user.
 * Simulates CPU work and schedules processes using RR algorithm.
 * Once done, prints a list of processes in the order they were finished. 
 * */
int main(int argc, char *argv[]) {

    return executeSchedulingAlgorithm(*roundRobin, argc, argv);
}




