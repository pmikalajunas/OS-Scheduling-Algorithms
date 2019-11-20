// -------------------- STRUCTURES -------------------- 
typedef struct Process
{
    int pId;
    int burstTime;
    int arrivalTime;
    int completionTime;
    int turnAroundTime;
    int waitingTime;
    
} Process;

typedef struct ComparisonData
{
    int averageTurnaroundTime;
    int averageWaitingTime;
    int throughput;
} ComparisonData;
// --------------------------------------------------- 