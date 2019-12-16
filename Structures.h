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
    int responseTime;
} Process;

typedef struct ComparisonData
{
    float averageBurstTime;
    float averageArrivalTime;
    float averageTurnaroundTime;
    float averageWaitingTime;
    float averageCompletionTime;
    float averageResponseTime;
    float throughput;
} ComparisonData;
// --------------------------------------------------- 