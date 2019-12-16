
// -------------------- CONSTANTS --------------------
int const TIME_QUANTUM = 10; // Fixed time quantum.
int const INCREASE_FACTOR = 2; // Factor by which queue length increases.
int const INPUT_ERROR = 1; // Input error code, if the cmd input is incorrect.
int const SUCCESSFUL_EXECUTION = 0; // Returned with successful execution

// -------------------- CONSTRAINTS --------------------
int const MAX_ARRIVAL_TIME = 2000;
int const MAX_BURST_TIME = 2000;
int const PROCESS_LIMIT = 100; 