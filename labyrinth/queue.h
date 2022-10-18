#ifndef QUEUE_H
#define QUEUE_H

// The structure Queue is a linked list of Nodes.
// Node stores element's value and a pointer to the next element.
typedef struct Node *Node;
typedef struct Queue *Queue;

// An auxiliary function to create new node with value [val].
Node create_node(size_t val);

// An auxiliary function to create an empty queue.
Queue create_queue();

// Function returns true if queue is empty.
bool empty(Queue queue);

// Function adds element with value [val] at the end of the queue.
void push(Queue queue, size_t val);

// Function returns value of the first element in the queue.
size_t front(Queue queue);

// Function removes the element on the front of the queue.
void pop(Queue queue);

// Function returns value of the last element in the queue.
size_t last(Queue queue);

// Function deallocates the memory previously used by the queue.
void free_queue(Queue queue);

#endif