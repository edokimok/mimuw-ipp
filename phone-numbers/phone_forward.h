/** @file
 * An interface for a class that stores phone number forwarding
 *
 * @authors Marcin Peczarski <marpe@mimuw.edu.pl>,
 * Emilia Dębicka <ed438406@students.mimuw.edu.pl>
 * @copyright University of Warsaw
 * @date 2022
 */

#ifndef __PHONE_FORWARD_H__
#define __PHONE_FORWARD_H__

#include <stddef.h>
#include <stdbool.h>

/**
 * This is a structure that stores phone number forwarding.
 */
typedef struct PhoneForward PhoneForward;

/**
 * This is a structure that stores a sequence of phone numbers.
 */
typedef struct PhoneNumbers PhoneNumbers;

/** @brief Creates a new structure.
 * Creates a new structure containing no redirections.
 * @return A pointer to the created structure, or NULL if failed to
 * allocate memory.
 */
PhoneForward *phfwdNew(void);

/** @brief Deletes the structure.
 * Deletes the structure pointed to by @p pf. Does nothing if this pointer has a
 * NULL value.
 * @param[in] pf - pointer to the structure to be removed.
 */
void phfwdDelete(PhoneForward *pf);

/** @brief Adds redirection.
 * Adds a redirection of all numbers having the prefix @p num1, to numbers
 * with that prefix replaced with the @p num2 prefix, respectively. Each number
 * is its own prefix. If a redirection has been previously added
 * with the same parameter @p num1, it is replaced.
 * The number forwarding relation is not transitive.
 * @param[in,out] pf - a pointer to a structure storing the redirection
 * numbers;
 * @param[in] num1 - a pointer to a string representing the prefix of the numbers
 * redirected;
 * @param[in] num2 - pointer to a string representing the prefix of the numbers,
 * to which the forwarding is made.
 * @return Value @p true if the redirection has been added.
 * Value @p false if an error occurred, e.g. the given string does not
 * represents a number, the two specified numbers are identical, @p pf is NULL or the function
 * failed to allocate memory.
 */
bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2);

/** @brief Removes redirections.
 * Removes all redirections where the @p num parameter is a prefix
 * of the @p num1 parameter used when adding. If there are no such redirects,
 * @p pf is NULL or the string does not represent a number, it does nothing.
 * @param[in,out] pf - a pointer to a structure that stores phone forwarding;
 * @param[in] num - a pointer to the string representing the number prefix.
 */
void phfwdRemove(PhoneForward *pf, char const *num);

/** @brief Calculates the forwarding of the number.
 * Calculates the forwarding of the specified number. Looks for the longest matching
 * prefix for the forwarding of @p num. The result is a string containing at most one number. If a given
 * number has not been forwarded, the result is a string containing that number.
 * If the given prefix does not represent a number, the result is an empty string.
 * If @p pf is NULL, return NULL.
 * Allocates the @p PhoneNumbers structure, which must be freed using the
 * the @ref phnumDelete function.
 * @param[in] pf - a pointer to a structure that stores number redirections;
 * @param[in] num - a pointer to a string representing a number.
 * @return A pointer to a structure storing a string of numbers or NULL when failed to allocate memory.
 * NULL if @p pf is NULL.
 */
PhoneNumbers *phfwdGet(PhoneForward const *pf, char const *num);

/** @brief Calculates the redirections to a given number.
 * Returns the following sequence of numbers: if there is a number @p x that has been forwarded to
 * any prefix of @p num, then the result of a call @ref phfwdReverse with number @p num contains
 * the number @p num with prefix replaced from @p x->forward to @p x. In addition,
 * the resulting sequence always contains the number @p num. The result is sorted
 * lexicographically and the numbers cannot repeat. If the given caption does not
 * represent a number, the result is an empty string. Function allocates the structure of
 * @p PhoneNumbers, which must be released using the @ref phnumDelete function.
 * @param[in] pf - a pointer to a structure that stores number redirections;
 * @param[in] num - a pointer to a caption representing the number.
 * @return A pointer to a structure storing a number string, or NULL if the function
 * failed to allocate memory or @p pf is NULL.
*/
PhoneNumbers *phfwdReverse(PhoneForward const *pf, char const *num);

/** @brief Function determines the counter image of the function @ref phfwdGet.
 * Returns the sequence of sorted lexicographically numbers @p x, that the result
 * of @ref phfwdGet(@p x) = @p num. Function allocates the structure of
 * @p PhoneNumbers, which must be released using the @ref phnumDelete function.
 * Returns NULL if failed to allocate memory. Returns an empty string is @p num is
 * not a phone number.
 * @param[in] pf - a pointer to a structure that stores number redirections;
 * @param[in] num - a pointer to a caption representing the number.
 * @return A pointer to a structure storing a number string or an empty string
 * if @p num is not a phone number; NULL if the function failed to allocate
 * memory or @p pf is NULL.
 */
PhoneNumbers *phfwdGetReverse(PhoneForward const *pf, char const *num);

/** @brief Deletes the structure.
 * Deletes the structure indicated by @p pnum. Does nothing if this pointer has a
 * NULL value.
 * @param[in] pnum - pointer to the structure being removed.
*/
void phnumDelete(PhoneNumbers *pnum);

/** @brief Returns a number.
* Returns a pointer to the string representing the number. The captions are indexed
 * sequentially from zero.
 * @param[in] pnum - a pointer to a structure that stores a string of phone numbers;
 * @param[in] idx - an index of the phone number.
 * @return A pointer to a string representing the phone number. NULL value if
 * @p pnum pointer has NULL value or index is too big.
*/
char const *phnumGet(PhoneNumbers const *pnum, size_t idx);

/**
 * This is an auxiliary function that converts a char containing a digit
 * to the corresponding int. Character "*" represents 10, and "#" represents 11.
 * @param[in] c - a character describing a digit;
 * @return An integer corresponding to character @p c. Value -1 if c is not
 * a phone number digit.
 */
static inline int charToDigit(char c) {
    if ('0' <= c && c <= '9')
        return c - '0';
    else if (c == '*')
        return 10;
    else if (c == '#')
        return 11;
    else
        return -1;
}

/**
 * This is an auxiliary function that converts a phone number digit to
 * to the corresponding char. Character "*" represents 10, and "#" represents 11.
 * @param[in] i - an integer describing a digit;
 * @return A character corresponding to integer @p i. Character '!' if i is not
 * a phone number digit.
 */
static inline char digitToChar(int i) {
    if (0 <= i && i <= 9)
        return i + '0';
    else if (i == 10)
        return '*';
    else if (i == 11)
        return '#';
    else
        return '!';
}

/**
 * This is an auxiliary function that checks if a char is a phone number digit
 * (a number from 0 to 9, a character * (representing digit 10) or # (representing 11)).
 * @param[in] c - a character describing a digit;
 * @return True if @p c is a number from 0 to 9, a character * or #. False otherwise.
 */
static inline bool isDigit(char c) {
    return ('0' <= c && c <= '9') || c == '*' || c == '#';
}

/**
 * This is an auxiliary function that compares phone number digits. It returns a differerence
 * @p a - @p b.
 * @param[in] a - first digit;
 * @param[in] b - second digit;
 * @return Positive integer if @p a > @p b, negative if @p a < @p b, 0 otherwise.
 */
static inline int compareDigits(char a, char b) {
    return charToDigit(a) - charToDigit(b);
}

#endif /* __PHONE_FORWARD_H__ */
