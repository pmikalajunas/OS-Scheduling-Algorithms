

// -------------------- LINKED LIST --------------------

typedef struct Node {
    Process *process;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct LinkedList {
    Node *head;
    Node *tail;
} LinkedList;

/* create a new node */
/* returns a pointer to the newly created node */
/* print an error message and return NULL if an error occurs */
Node *initialise_node(void)
{
    Node *newNode = (Node*) malloc(sizeof(Node));
    if(!newNode){
        fprintf(stderr, "(initialise_node) Malloc failed at line: (%d)\n", __LINE__);
        return NULL;
    }

    //newNode->process = NULL;
    newNode->next = NULL;
    newNode->prev = NULL;

    return newNode;
}

/* True - linked list is empty */
/* False - linked list is not empty */
bool linked_list_empty(LinkedList *list) {
    if(!list->head) {
        return true;
    }
    return false;
}

/* free memory for node *node */
/* print an error message and return if node is NULL */
void free_node(Node *node)
{
    if(!node){
        fprintf(stderr, "(free_node) Node was NULL at line: (%d)\n", __LINE__);
        return;
    }

    free(node);
}

/* create a new linked list */
/* returns a pointer to the newly created list */
/* print an error message and return NULL if an error occurs */
LinkedList *initialise_linked_list(void)
{
    LinkedList *newList = (LinkedList*) malloc(sizeof(LinkedList));

    if(!newList){
        fprintf(stderr, "(initialise_linked_list) Malloc failed at line: (%d)\n", __LINE__);
        return NULL;
    }

    newList->head = NULL;
    newList->tail = NULL;
    return newList;
}

/* create and add node to the tail of linked list *list */
/* and set data stored at node to *data */
/* should return a pointer to the new node */
/* should return NULL if an error occurs */
Node *append_linked_list(LinkedList *list, Process *process)
{
    Node* newNode;

    /* is list NULL??? is data null???*/
    if(!list){
        fprintf(stderr, "(append_linked_list) (List/data) was(were) NULL at line: (%d)\n", __LINE__);
        return NULL;
    }

    /* Initializing data */
    newNode = initialise_node();
    if(!newNode){
        fprintf(stderr, "(append_linked_list) Failure initializing new node at line: (%d)\n", __LINE__);
        return NULL;
    }

    /* setting the passed data */
    newNode->process = process;

    /* if the list is empty */
    if(!list->head || !list->tail){
        /* pointing our head and tail to the new node... */
        list->head = newNode;
        list->tail = newNode;
        return newNode;
    }

    /* linking the node which was last */
    list->tail->next = newNode;
    newNode->prev = list->tail;

    /* Moving our tail pointer */
    list->tail = newNode;

    return newNode;
}

void merge_linked_lists(LinkedList *first_list, LinkedList *second_list) {
    first_list->tail->next = second_list->head;
    second_list->head->prev = first_list->tail;
    first_list->tail = second_list->tail;
    first_list->tail->next = NULL;
}

/* remove head from linked list *list */
/* print an error message and return if list is NULL or empty */
Node *remove_head_linked_list(LinkedList *list)
{
    /* is list NULL??? is it empty?? */
    if(!list || !list->head){
        return NULL;
    }

    /* if we are left with one element... */
    if(!list->head->next){
        Node *node = list->head;
        list->head = NULL;
        list->tail = NULL;
        return node;
    }

    /* Pointing head to the next node */
    list->head = list->head->next;

    /* freeing the garbage */
    Node *node = list->head->prev;

    /* first node doesn't have previous node... */
    list->head->prev = NULL;

    return node;
}

/* Returns the head of the linked list */
/* Head is not discarded from the linked list! */
/* print an error message and return if list is NULL or empty */
Node *peek_head_linked_list(LinkedList *list)
{
    /* is list NULL??? is it empty?? */
    if(!list || !list->head){
        return NULL;
    }
    return list->head;
}



/* free memory for linked list *list */
/* frees memory for all nodes in linked list and list itself */
/* print an error message and return if list is NULL */
void free_linked_list(LinkedList *list)
{
    /* is list NULL??? */
    if(!list){
        fprintf(stderr, "(free_linked_list) List was NULL at line: (%d)\n", __LINE__);
        return;
    }

    /* freeing each node */
    while(list->head){
        Node *temp = list->head;
        list->head = list->head->next;
        /* freeing the node */
        free_node(temp);
    }

    /* ant the list itself... */
    free(list);
}
