#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "queue.h"

struct Node {
    size_t value;
    Node next;
};

// Members [first] and [last] indicate successively the first and
// the last element added to the queue.
struct Queue {
    Node first;
    Node last;
};

Node create_node(size_t val){
    Node node = malloc(sizeof(struct Node));
    node->value = val;
    node->next = NULL;
    return node;
}

Queue create_queue() {
    Queue q = malloc(sizeof(struct Queue));
    q->first = NULL;
    q->last = NULL;
    return q;
}

bool empty(Queue queue) {
    return queue->first == NULL;
}

void push(Queue queue, size_t val) {
    Node node = create_node(val);
    // If the queue was empty, added node is the only element of the queue.
    if (empty(queue)) {
        queue->first = node;
        queue->last = node;
    }
    else {
        Node prev_last = queue->last;
        prev_last->next = node;
        queue->last = node;
    }
}

// Following functions front, pop and last assume that
// given queue is not empty.
size_t front(Queue queue) {
    Node node = queue->first;
    return node->value;
}

void pop(Queue queue) {
    Node prev_first = queue->first;
    Node new_first = prev_first->next;
    queue->first = new_first;
    free(prev_first);
}
;
size_t last(Queue queue) {
    Node node = queue->last;
    return node->value;
}

void free_queue(Queue queue) {
    while(!empty(queue)) {
        pop(queue);
    }
    free(queue);
}