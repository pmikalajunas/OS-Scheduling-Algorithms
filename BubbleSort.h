
/**
 * Swaps processes between two nodes of the linkedList.
 * */
void swap(Node *a, Node *b) 
{ 
    Process *temp = a->process; 
    a->process = b->process; 
    b->process = temp; 
} 


/**
 * Implementation of Bubble Sort for Linked List.
 * Sorts processes by their arrival time in ascending order.
 * */
void bubbleSort(Node *start) 
{ 
    int swapped, i; 
    Node *ptr1; 
    Node *lptr = NULL; 
  
    // If list is empty, we don't move any further.
    if (start == NULL) {
        return; 
    }
        
    do
    { 
        swapped = 0; 
        ptr1 = start; 
  
        while (ptr1->next != lptr) 
        { 
            if (ptr1->process->arrivalTime > ptr1->next->process->arrivalTime) 
            {  
                swap(ptr1, ptr1->next); 
                swapped = 1; 
            } 
            ptr1 = ptr1->next; 
        } 
        lptr = ptr1; 
    } 
    while (swapped); 
} 
  