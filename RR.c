


int const timeQuantum = 20; // Fixed time quantum

struct process
{
    int pId;
    int burstTime;
};

struct process blocked[]; // - may not need.
struct process ready[];
struct process running[1]; 

