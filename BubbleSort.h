
/* function to swap data of two nodes a and b*/
void swap(Node *a, Node *b) 
{ 
    Process *temp = a->process; 
    a->process = b->process; 
    b->process = temp; 
} 


void bubbleSort(Node *start) 
{ 
    int swapped, i; 
    Node *ptr1; 
    Node *lptr = NULL; 
  
    /* Checking for empty list */
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
  