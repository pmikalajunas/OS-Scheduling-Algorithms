


int const timeQuantum = 20; // Fixed time quantum

struct process
{
    int pId;
    int burstTime;
};

int blocked[];
int ready[];
int running[1]; 

