/** @file
 * The main module of a class that implements a stack
 *
 * @author Emilia Dębicka <ed438406@students.mimuw.edu.pl>
 * @copyright University of Warsaw
 * @date 2022
 */

#include "stack.h"
#include <stdlib.h>

/**
 * The structure of a stack.
 */
struct Stack {
    PhoneForward *digit; /**< A pointer to the PhoneForward structure. */
    Stack *next; /**< A pointer to the next element. */
};

Stack *newNode(void) {
    Stack *st;
    st = (Stack *) malloc(sizeof(Stack));
    if (st != NULL) {
        st->next = NULL;
        st->digit = NULL;
    }

    return st;
}

bool is_empty(Stack const *st) {
    return (st == NULL || st->next == NULL);
}

void push(Stack **st, PhoneForward *digit) {
    Stack *new = newNode();
    if (new != NULL) {
        new->next = *st;
        new->digit = digit;
        *st = new;
    }
}

void pop(Stack **st) {
    if (!is_empty(*st)) {
        Stack *temp = *st;
        *st = (*st)->next;
        free(temp);
    }
}

PhoneForward *top(Stack const *st) {
    return st->digit;
}

void removeStack(Stack **st) {
    if (st != NULL) {
        while (!is_empty(*st)) {
            pop(st);
        }
        free(*st);
    }
}