#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "./LinkedList.h"
#include "./SchedulerHelper.h"

int main(){
LinkedList *queue1 = initialise_linked_list();
LinkedList *queue2 = initialise_linked_list();

Process *process1 = newProcess(1, 1);
Process *process2 = newProcess(2, 2);
Process *process3 = newProcess(3, 2);
Process *process4 = newProcess(4, 2);
Process *process5 = newProcess(5, 2);
Process *process6 = newProcess(6, 2);
Process *process7 = newProcess(7, 2);
Process *process8 = newProcess(8, 2);
Process *process9 = newProcess(9, 2);

append_linked_list(queue1, process1);
append_linked_list(queue1, process2);
append_linked_list(queue1, process3);
append_linked_list(queue1, process4);
append_linked_list(queue2, process5);
append_linked_list(queue2, process6);
append_linked_list(queue2, process7);
append_linked_list(queue2, process8);
append_linked_list(queue2, process9);

merge_linked_lists(queue1, queue2);

printf("%d\n", queue1->tail->process->burstTime);
return 0;
}

