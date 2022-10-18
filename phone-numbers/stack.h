/** @file
 * An interface for a class implementing a stack
 *
 * @author Emilia Dębicka <ed438406@students.mimuw.edu.pl>
 * @copyright University of Warsaw
 * @date 2022
 */

#ifndef __STACK_H__
#define __STACK_H__

#include "phone_forward.h"

/**
 * This is a structure of a stack.
 */
typedef struct Stack Stack;

/** Function creates a new stack.
 * Function creates a new empty stack.
 * @return A pointer to the new stack.
 */
Stack *newNode(void);

/** Function checks if the stack is empty.
 * Function ckecks if the givem stack is empty.
 * @param[in] st - a pointer to the stack;
 * @return Value @p true if the stack is empty, @p false otherwise.
 */
bool is_empty(Stack const *st);

/** Function adds an element to the stack.
 * Function creates a new element containing @p digit with function @ref newNode and adds it
 * at the top of the stack.
 * @param[in, out] st - a pointer to the stack;
 * @param[in, out] digit - the information that the new node will contain.
 */
void push(Stack **st, PhoneForward *digit);

/** Function removes an element of the stack.
 * Function removes an element at the top of the stack. It does nothing if the stack is empty.
 * @param[in, out] st - a pointer to the stack.
 */
void pop(Stack **st);

/** Function returns the element at the top of the stack.
 * Function returns the information about the element, which is at the top of the stack.
 * It can't be called on an empty stack.
 * @param[in] st - a pointer to the stack; the stack can not be empty;
 * @return The information contained in the element at the top of the stack.
 */
PhoneForward *top(Stack const *st);

/** Function deletes the whole stack.
 * Function deletes the structure and frees the allocated memory.
 * @param[in, out] st - a pointer to the structure to be deleted.
 */
void removeStack(Stack **st);

#endif /* __STACK_H__ */