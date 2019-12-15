// -------------------- STRUCTURES -------------------- 
typedef struct Process
{
    int pId;
    int burstTime;
    int arrivalTime;
    int completionTime;
    int turnAroundTime;
    int waitingTime;
    int timeSpentProcessing;
    int remainingTime;
} Process;

typedef struct ComparisonData
{
    float averageBurstTime;
    float averageArrivalTime;
    float averageTurnaroundTime;
    float averageWaitingTime;
    float averageCompletionTime;
} ComparisonData;
// --------------------------------------------------- 