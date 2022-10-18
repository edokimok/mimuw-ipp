/** @file
 * The main module of a class that stores phone number forwarding
 *
 * @author Emilia Dębicka <ed438406@students.mimuw.edu.pl>
 * @copyright University of Warsaw
 * @date 2022
 */

#include "stack.h"
#include "phone_forward.h"
#include <stdlib.h>
#include <string.h>

/**
 * A macro that informs how many different characters can follow a digit.
 */
#define NUMBER_OF_DIGITS 12

/**
 * A macro that stores the inital size of pnum numbers array.
 */
#define INITIAL_NUMBERS_SIZE 4

/**
 * A macro that informs what is the initial index value of index of the element
 * of PhoneForward structure.
 */
#define INITIAL_PF_INDEX -1

/**
 * The structure stores phone number forwarding.
 */
struct PhoneForward {
    /**@{*/
    PhoneForward *nextDigitsArray[NUMBER_OF_DIGITS]; /**< An array of the next possible digits. */
    PhoneForward *prevDigit; /**< A pointer to the previous digit. */

    /** An integer describing how many cells in @p nextDigitsArray are occupied. */
    int numberOfNextDigits;

    /** A value of the digit (a node of PhoneForward). */
    int index;

    /** Forwarding from numbers with prefix @p num1 to numbers with prefix changed to @p num2. \n
    * @p digit->@p forward != NULL when there is a redirection from numbers with prefix finished in @p digit
    * (@p num1[@p digit] is the last character of @p num1). \n
    * Then @p digit->@p forward = @p num2.
    */
    char *forward;
    /**@}*/
};

/**
 * The structure stores a sequence of phone numbers.
 */
struct PhoneNumbers {
    /**@{*/
    char **numbers; /**< An array of phone numbers. */
    size_t numbersSize; /**< The size of @p numbers array. */
    size_t numbersCount; /**< The number of numbers stored in @p numbers array. */
};

/** Function creates a new PhoneNumbers structure.
 * Function creates a new PhoneNumbers structure with no numbers and sets the initial field values.
 * The initial size of @p numbers array is 4.
 * @return A pointer to the created structure or NULL if failed to allocate memory.
 */
static PhoneNumbers *pnumNew(void) {
    PhoneNumbers *pnum = NULL;
    pnum = (PhoneNumbers *) malloc(sizeof(PhoneNumbers));

    if (pnum != NULL) {
        pnum->numbersCount = 0;
        pnum->numbersSize = INITIAL_NUMBERS_SIZE;
        pnum->numbers = malloc(pnum->numbersSize * sizeof(char *));
    }

    return pnum;
}

PhoneForward *phfwdNew(void) {
    PhoneForward *pf = NULL;
    pf = (PhoneForward *) malloc(sizeof(PhoneForward));

    if (pf) {
        pf->forward = NULL;
        pf->index = INITIAL_PF_INDEX;
        pf->numberOfNextDigits = 0;
        pf->prevDigit = NULL;
        for (int i = 0; i < NUMBER_OF_DIGITS; i++) {
            pf->nextDigitsArray[i] = NULL;
        }
    }

    return pf;
}

void phfwdDelete(PhoneForward *pf) {
    if (pf != NULL) {
        Stack *st = newNode();
        if (st == NULL) {
            return;
        }
        push(&st, pf);

        while (!is_empty(st)) {
            PhoneForward *node = top(st);
            if (node->numberOfNextDigits == 0) {
                pop(&st);
                if (node->index != -1) {
                    (node->prevDigit)->nextDigitsArray[node->index] = NULL;
                }
                free(node->forward);
                free(node);
            } else {
                int i = 0;
                while (i < NUMBER_OF_DIGITS && node->numberOfNextDigits > 0) {
                    if (node->nextDigitsArray[i] != NULL) {
                        push(&st, node->nextDigitsArray[i]);
                        node->numberOfNextDigits--;
                    }
                    i++;
                }
            }
        }
        removeStack(&st);
    }
}

/** Function returns the length of a number.
 * Function checks if given string is correct (if it contains of digits only and ends with character '\0')
 * and if so, it returns its length.
 * @param[in] num - a pointer to a string representing the number;
 * @return The length of @p num or 0 if the given string @p num is incorrect.
 */
static size_t length(char const *num) {
    if (num == NULL) {
        return 0;
    }
    size_t i = 0;
    while (num[i] != '\0') {
        if (!isDigit(num[i])) {
            return 0;
        }
        i++;
    }
    return i;
}

/** Function marks the end of the prefix @p num1 with corresponding new prefix @p num2.
 * Function sets the forwarding to @p num1 used in @ref phfwdAdd function.
 * @param[in, out] node - a pointer to the @p PhoneNumbers node describing the last character of @p num1;
 * @param[in] forwardNum - a pointer to the new prefix.
 * @return Value @p false if failed to allocate memory. Otherwise @p true.
 */
static bool mark(PhoneForward *node, char const *forwardNum) {
    size_t forwardNumSize = length(forwardNum);
    node->forward = malloc((forwardNumSize + 1) * sizeof(char));
    if (node->forward == NULL) {
        return false;
    }
    memcpy(node->forward, forwardNum, (forwardNumSize + 1) * sizeof(char));
    return true;
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2) {
    if (pf == NULL) {
        return false;
    }
    PhoneForward *node = pf;

    // Function returns false if any of the numbers ([num1], [num2]) is incorrect,
    // or if they are the same number.
    if (length(num1) == 0 || length(num2) == 0 || strcmp(num1, num2) == 0) {
        return false;
    }

    size_t i = 0;
    while (num1[i] != '\0') {
        int index = charToDigit(num1[i]);

        if (node->nextDigitsArray[index] == NULL) {
            node->nextDigitsArray[index] = phfwdNew();
            // Checking if [node->nextDigitsArray[index]] was allocated properly.
            if (node->nextDigitsArray[index] == NULL) {
                return false;
            }
            node->nextDigitsArray[index]->prevDigit = node;
            node->nextDigitsArray[index]->index = index;
            node->numberOfNextDigits++;
        }
        node = node->nextDigitsArray[index];
        i++;
    }

    free(node->forward);
    return mark(node, num2);
}

void phfwdRemove(PhoneForward *pf, char const *num) {
    if (pf != NULL) {
        PhoneForward *node = pf;
        size_t numSize = length(num);

        /* The last significant node (it has a forwarding,
         * or it is the prefix of more than one forwarded number). */
        PhoneForward *lastSigNode = pf;
        int deleteIndex = 0; // An index of the first of non-significant nodes.

        if (numSize != 0) {
            for (size_t i = 0; i < numSize; i++) {
                int index = charToDigit(num[i]);

                if (node->nextDigitsArray[index] == NULL) {
                    break;
                }

                if (node->numberOfNextDigits > 1 || node->forward != NULL || i == 0) {
                    lastSigNode = node;
                    deleteIndex = index;
                }
                node = node->nextDigitsArray[index];
            }
            if (lastSigNode->nextDigitsArray[deleteIndex] != NULL) {
                lastSigNode->numberOfNextDigits--;
                phfwdDelete(lastSigNode->nextDigitsArray[deleteIndex]);
                lastSigNode->nextDigitsArray[deleteIndex] = NULL;
            }
        }
    }
}

/** Function adds a number to PhoneNumbers.
 * Function reallocates the PhoneNumbers structure if needed and adds a new number @p num.
 * @param[in, out] pn - a pointer to the PhoneNumbers structure;
 * @param[in] num - a pointer to the number to be added.
 */
static void addPhoneNumber(PhoneNumbers *pn, char const *num) {
    if (pn == NULL) {
        return;
    }
    if (pn->numbersCount == pn->numbersSize) {
        pn->numbersSize *= 2;
        pn->numbers = realloc(pn->numbers, pn->numbersSize * sizeof(char *));
    }
    if (pn->numbers == NULL) {
        return;
    }
    size_t numSize = length(num);
    if (numSize != 0) {
        pn->numbers[pn->numbersCount] = malloc((numSize + 1) * sizeof(char));
        if (pn->numbers[pn->numbersCount] == NULL) {
            return;
        }
        memcpy(pn->numbers[pn->numbersCount], num, numSize + 1);
    } else {
        pn->numbers[pn->numbersCount] = NULL;
    }
    pn->numbersCount++;
}

/** Function merges prefix with the rest of the number,
 * Function creates the forwarded number (the result of @ref phfwdGet function) by merging
 * the @p newPrefix with the rest of the number @p num.
 * @param[in] num - a pointer to the number;
 * @param[in] forwarded - a boolean informing if the number has been forwarded;
 * @param[out] newNumber - a pointer to the forwarding number;
 * @param[in] newPrefix - a pointer to the new prefix;
 * @param[in] prefixIndex - a variable thet informs how many digits of @p num has to be forwarded.
 */
static void mergePrefNum(char const *num, bool forwarded, char **newNumber, char const *newPrefix, size_t prefixIndex) {
    char *temp = *newNumber;
    size_t numSize = length(num);
    size_t prefixSize = length(newPrefix);
    if (!forwarded) {
        temp = calloc((numSize + 1), sizeof(char));
        if (temp == NULL) {
            return;
        }
        memcpy(temp, num, numSize + 1);
    } else {
        size_t newNumberSize = numSize + prefixSize - prefixIndex - 1;
        temp = calloc((newNumberSize + 1), sizeof(char));
        if (temp == NULL) {
            return;
        }

        memcpy(temp, newPrefix, prefixSize + 1);

        for (size_t i = 0; i < numSize - prefixIndex - 1; i++) {
            temp[prefixSize + i] = num[prefixIndex + i + 1];
        }
    }
    *newNumber = temp;
}

/** Function finds the longest prefix of @p num that has a redirection.
 * Function traverses the @p PhoneForward structure an searches for the longest prefix of @p num
 * that has been forwarded. It merges the new prefix with the rest of the number with @ref mergePrefNum.
 * @param[in] node - a pointer to the PhoneForward structure;
 * @param[in] num - a pointer to the given number;
 * @param[out] newNumber - a pointer to the forwarding number.
 */
static void findPrefix(PhoneForward const *node, char const *num, char **newNumber) {
    bool forwarded = false;
    char *newPrefix;
    size_t numSize = length(num);
    size_t prefixSize = 0;
    newPrefix = calloc((prefixSize + 1), sizeof(char));
    if (newPrefix == NULL) {
        return;
    }
    size_t prefixIndex = 0; // The index with the longest forwarded prefix.

    for (size_t i = 0; i < numSize; i++) {
        int index = charToDigit(num[i]);

        if (node->nextDigitsArray[index] == NULL) {
            break;
        }

        node = node->nextDigitsArray[index];

        if (node->forward != NULL) {
            size_t newPrefixSize = length(node->forward);
            newPrefix = realloc(newPrefix, newPrefixSize + 1);
            if (newPrefix == NULL) {
                return;
            }
            prefixSize = newPrefixSize;
            memcpy(newPrefix, node->forward, prefixSize + 1);
            prefixIndex = i;
            forwarded = true;
        }
    }

    mergePrefNum(num, forwarded, newNumber, newPrefix, prefixIndex);
    free(newPrefix);
}

PhoneNumbers *phfwdGet(PhoneForward const *pf, char const *num) {
    if (pf == NULL) {
        return NULL;
    }

    size_t numSize = length(num);
    PhoneNumbers *pnum = pnumNew();
    PhoneForward const *node = pf;
    char *newNumber = NULL; // Forwarding number.

    if (numSize != 0) {
        findPrefix(node, num, &newNumber);
    }
    addPhoneNumber(pnum, newNumber);
    if (numSize != 0 && newNumber != NULL) {
        free(newNumber);
    }

    return pnum;
}

/** Function comapres two strings. Returns positive integer if the first string is larger, negative if smaller,
 * and 0 if they are equal.
 * @param[in] a - a pointer to the first string;
 * @param[in] b - a pointer to the second string;
 * @return 1 if @p a > @p b, -1 if @p a < @p b, 0 otherwise.
 */
static int compareString(char const *a, char const *b) {
    size_t lenA = length(a);
    size_t lenB = length(b);

    for (size_t i = 0; i < lenA; i++) {
        if (i == lenB) {
            return 1;
        }
        if (compareDigits(a[i], b[i]) != 0) {
            return compareDigits(a[i], b[i]);
        }
    }
    if (lenA < lenB) {
        return -1;
    } else {
        return 0;
    }
}

/** Comaparator function for qsort. Returns positive integer if the first string is larger, negative if smaller,
 * and 0 if they are equal.
 * @param[in] a - a pointer to the first string;
 * @param[in] b - a pointer to the second string;
 * @return Positive integer if @p a > @p b, negative if @p a < @p b, 0 otherwise.
 */
static int compare(const void *a, const void *b) {
    const char **stringA = (const char **) a;
    const char **stringB = (const char **) b;

    return compareString(*stringA, *stringB);
}

/** Function checks if @p forwarding is the prefix of @p num and if so, it creates the number that is
 * one of the results of @ref phfwdReverse function and saves it in @p newString. Otherwise it does nothing.
 * @param[in] prefix - a pointer to the perfix of the new number;
 * @param[in] prefixSize - an integer describing the number of significant digits in prefix;
 * @param[in] forwarding - a pointer to the forwarding;
 * @param[in] num - a pointer to the number given in @ref phfwdReverse;
 * @param[out] newString - a pointer to the new number.
 */
static void merge(char **newString, char const *prefix, size_t prefixSize, char const *forwarding, char const *num) {
    size_t numSize = length(num);
    size_t forwardSize = length(forwarding);
    size_t newStringSize = numSize - forwardSize + prefixSize;
    if (numSize == 0) {
        return;
    }
    bool isPrefix = false;
    for (size_t i = 0; i < numSize; i++) {
        if (forwardSize - i - 1 == 0) {
            if (compareDigits(num[i], forwarding[i]) != 0) {
                break;
            }
            isPrefix = true;
            break;
        }
        if (compareDigits(num[i], forwarding[i]) != 0) {
            break;
        }
    }

    if (isPrefix == true) {
        *newString = calloc(newStringSize + 1, sizeof(char));
        if (*newString == NULL) {
            return;
        }
        memcpy(*newString, prefix, prefixSize + 1);
        (*newString)[prefixSize] = '\0';
        for (size_t i = prefixSize; i < newStringSize; i++) {
            (*newString)[i] = num[forwardSize + i - prefixSize];
        }
    }
}

/** Function removes the duplicates in @p array.
 * @param[in, out] array - a pointer to the array;
 * @param[in, out] size - an integer describing the size of an array.
 */
static void removeDuplicates(char ***array, size_t *size) {
    char **temp = calloc(*size + 1, sizeof(char *));
    size_t uniqueValues = 0;
    for (size_t i = 0; i < *size; i++) {
        if (i == 0) {
            temp[i] = (*array)[i];
            uniqueValues++;
        } else {
            if (strcmp((*array)[i], temp[uniqueValues - 1]) != 0) {
                temp[uniqueValues] = (*array)[i];
                uniqueValues++;
            } else {
                free((*array)[i]);
            }
        }
    }
    free(*array);
    *array = calloc(uniqueValues, sizeof(char *));
    for (size_t i = 0; i < uniqueValues; i++) {
        (*array)[i] = temp[i];
    }
    free(temp);
    *size = uniqueValues;
}

/** Function reallocates an @p array if needed.
 * @param[in, out] array - a pointer to the array;
 * @param[in] currentIndex - an integer describing the index of the last significant cell.
 * @param[in, out] maxSize - an integer describing the size of an array.
 */
static void safeRealloc(char **array, size_t currentIndex, size_t *maxSize) {
    if (currentIndex >= *maxSize) {
        *array = realloc(*array, (2 * (*maxSize)) * sizeof(char));
        *maxSize *= 2;
    }
}

/** Function traverses through PhoneForward structure and searches for the forwardigns specified in
 * @ref phfwdReverse function. It allocates the @p pnum structure.
 * @param[in, out] st - a pointer to the stack helpful with traversing;
 * @param[in] node - a pointer to the root of PhoneForward stucture;
 * @param[in] num - a pointer to the number given in @ref phfwdReverse function;
 * @param[out] pnum - a pointer to the PhoneNumbers structure.
 */
static void traversePhoneForward(Stack **st, PhoneForward const *node, char const *num, PhoneNumbers *pnum) {
    size_t depth = 0;
    char *newString = NULL;
    char *newPrefix = calloc((depth + 1), sizeof(char));
    if (newPrefix != NULL) {
        size_t newPrefixSize = depth + 1;
        PhoneForward *token = NULL;

        while (!is_empty(*st)) {
            node = top(*st);
            pop(st);
            if (node == NULL) {
                depth--;
            } else {
                if (node->index != -1) {
                    newPrefix[depth - 1] = digitToChar(node->index);
                }
                if (node->forward != NULL) {
                    free(newString);
                    newString = NULL;
                    merge(&newString, newPrefix, depth, node->forward, num);
                    if (newString != NULL) {
                        addPhoneNumber(pnum, newString);
                    }
                }
                if (node->numberOfNextDigits != 0) {
                    depth++;
                    safeRealloc(&newPrefix, depth, &newPrefixSize);
                    push(st, token);
                    int i = 0, k = 0;
                    while (i < NUMBER_OF_DIGITS && node->numberOfNextDigits - k > 0) {
                        if (node->nextDigitsArray[i] != NULL) {
                            push(st, node->nextDigitsArray[i]);
                            k++;
                        }
                        i++;
                    }
                }
            }
        }
        if (newString != NULL) {
            free(newString);
        }
        free(newPrefix);
    }
}

PhoneNumbers *phfwdReverse(PhoneForward const *pf, char const *num) {
    PhoneNumbers *pnum = pnumNew();
    if (pf == NULL || pnum == NULL) {
        phnumDelete(pnum);
        return NULL;
    } else {
        if (length(num) == 0) {
            addPhoneNumber(pnum, NULL);
            return pnum;
        }
        Stack *st = newNode();
        if (st == NULL) {
            return NULL;
        }
        // Casting pf to non-const to use the stack.
        PhoneForward *node = (PhoneForward *) pf;
        push(&st, node);
        traversePhoneForward(&st, pf, num, pnum);
        removeStack(&st);

        addPhoneNumber(pnum, num);
        qsort(pnum->numbers, pnum->numbersCount, sizeof(char *), compare);
        removeDuplicates(&(pnum->numbers), &(pnum->numbersCount));
        return pnum;
    }
}

PhoneNumbers *phfwdGetReverse(PhoneForward const *pf, char const *num) {
    if (pf == NULL) {
        return NULL;
    }
    PhoneNumbers *pnumReverse = phfwdReverse(pf, num);
    PhoneNumbers *pnumGetReverse = pnumNew();
    if (pnumGetReverse == NULL || pnumReverse == NULL) {
        phnumDelete(pnumReverse);
        return NULL;
    }

    PhoneNumbers *potentialNumber;
    for (size_t i = 0; i < pnumReverse->numbersCount; i++) {
        potentialNumber = phfwdGet(pf, phnumGet(pnumReverse, i));
        if (potentialNumber != NULL) {
            if (compareString(phnumGet(potentialNumber, 0), num) == 0) {
                addPhoneNumber(pnumGetReverse, phnumGet(pnumReverse, i));
            }
        }
        phnumDelete(potentialNumber);
    }

    phnumDelete(pnumReverse);

    return pnumGetReverse;
}

void phnumDelete(PhoneNumbers *pnum) {
    if (pnum != NULL) {
        if (pnum->numbers != NULL) {
            for (size_t i = 0; i < pnum->numbersCount; i++) {
                if (pnum->numbers[i] != NULL) {
                    free(pnum->numbers[i]);
                }
            }
            free(pnum->numbers);
        }
        free(pnum);
    }
}

char const *phnumGet(PhoneNumbers const *pnum, size_t idx) {
    if (pnum != NULL && idx < pnum->numbersCount) {
        return pnum->numbers[idx];
    } else {
        return NULL;
    }
}
