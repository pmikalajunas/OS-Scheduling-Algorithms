# OSC Coursework 1 - Scheduling Algorithms
## How to use the program
### Running the program
To run the pre-compiled program, simply run the `RR`, `FCFS` or `MLFQ` files, passing command-line arguments with the data you want to use. This data should be in the format of `(Burst time) (Arrival time) (Burst time) (Arrival time)...`.

For example:
```
RR 23 0 21 10 21 33
```
would run the round robin algorithm with the following data:

| Process | Burst time | Arrival time |
|---------|------------|--------------|
| p1 | 20 | 0 |
| p2 | 21 | 10 |
| p3 | 21 | 33 |

You can specify as much data as you want, but you have to pass an even number of arguments (Because every process needs both a burst time and an arrival time.)

To run either of the other two algorithms, simply replace `RR` with `FCFS` or `MLFQ`.

### Recompiling the program

#### Toggle CPU clock print statements 
It's possible to show or hide the complete process of how the algorithm calculates the final answers. This can be toggled using the `DEBUG` flag in `Debug.h`. If this flag is set to true, the state of the CPU at each clock tick will be printed along with the final tables. If it's set to false, only the final tables will be shown.

#### Change time quantum for round robin
The time quantum for round robin is stored in a integer constant in `SchedulerHelper.h` called `TIME_QUANTUM`. This value can be changed to use a different time quantum. To do this, simply change the value and then recompile the file.

#### Compilation

For compilation, we used GCC. Therefore, to recompile the file, we use these commands:
```
gcc -o RR RR.c
```
```
gcc -o FCFS FCFS.c
``` 
```
gcc -o MLFQ MLFQ.c
```

## Files in the repository
### Header Files
| File Name | Use |
|------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------|
| BubbleSort.h | Includes a bubble sort implementation which was used to order the process queue. |
| FCFS.h | Header file for first come first served algorithm. Contains the majority of the code for this algorithm. |
| MLFQ.h | Header file for the multi-level feedback queue algorithm. Contains most of the code for this algorithm. |
| RR.h | Header file for round robin algorithm. Contains most of the logic for this algorithm. |
| ScheduleHelper.h | General header file used in by all of the algorithms. Contains a series of helper functions for the scheduler algorithms. |
| Structures.h | General header file which contains the C Structures for each Process and a structure called ComparisonData to organise the statistics for the processes. |
| D.h | Contains the flag which determines if the information about each clock cycle is printed or not (if it's true, it's printed, else it's not). |

### C Files

| File | Use |
|--------|--------------------------------------------------------------------------|
| FCFS.c | Runs the FCFS scheduling algorithm using the FCFS.h file. |
| MLFQ.c | Runs the MLFQ scheduling algorithm using both the FCFS.h and RR.h files. |
| RR.c | Runs the RR scheduling algorithm using the RR.h file. |

### Compiled Files

| File | Use |
|------|-------------------------------------------|
| FCFS | Compiled file used to run FCFS algorithm. |
| MLFQ | Compiled file used to run MLFQ algorithm. |
| RR | Compiled file used to run RR algorithm. |



# Constraints

In our environment, we successfully tested data up to the following limits, and therefore we placed the following constraints to ensure the program remains stable. However, these numbers are just constants in ScheduleHelper.h and can be changed.

| Constraint                | Explanation                                                     | Name of Constant |
|---------------------------|-----------------------------------------------------------------|------------------|
| Burst time limit          | Maximum value set for burst time can be inputted by the user.   | MAX_BURST_TIME   |
| Arrival time limit        | Maximum value set for arrival time can be inputted by the user. | MAX_ARRIVAL_TIME |
| Number of processes limit | Maximum number of processes that can be inputted by the user.   | PROCESS_LIMIT    |

