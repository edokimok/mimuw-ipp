#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>
#include "queue.h"
#include "parse_input.h"

#define NUMBER_OF_BITS_IN_BYTE 8;
#define BASE 16;
#define TWO_POWER_32 0x100000000;

struct Labyrinth {
    size_t *dimensions_array;
    size_t dimensions_number, size, bits_number;
    unsigned char *bits_array;
    Queue queue;
};

size_t *get_dimensions_array(Labyrinth lab) {
    return lab->dimensions_array;
}

size_t read_dimensions_array(Labyrinth lab, size_t index) {
    return (lab->dimensions_array)[index];
}

size_t get_dimensions_number(Labyrinth lab) {
    return lab->dimensions_number;
}

size_t get_size(Labyrinth lab) {
    return lab->size;
}

size_t get_bits_number(Labyrinth lab) {
    return lab->bits_number;
}

void set_bits_number(Labyrinth lab, size_t value) {
    lab->bits_number = value;
}

unsigned char *get_bits_array(Labyrinth lab) {
    return lab->bits_array;
}

void create_bits_array(Labyrinth lab) {
    lab->bits_array = calloc(lab->bits_number, sizeof(unsigned char));
}

Queue get_queue(Labyrinth lab) {
    return lab->queue;
}

void free_all(Labyrinth lab) {
    free(lab->dimensions_array);
    free(lab->bits_array);
    free_queue(lab->queue);
    free(lab);
}

Labyrinth create_labyrinth() {
    Labyrinth lab = malloc(sizeof(struct Labyrinth));
    lab->dimensions_array = malloc(16 * sizeof(size_t));
    lab->dimensions_number = 0;
    lab->size = 1;
    lab->queue = create_queue();
    lab->bits_array = NULL;
    return lab;
}


void error(Labyrinth lab, int error_number) {
    fprintf(stderr, "ERROR %d\n", error_number);
    if (lab->dimensions_array != NULL)
        free(lab->dimensions_array);
    if (lab->bits_array != NULL)
        free(lab->bits_array);
    free_queue(lab->queue);
    free(lab);
    exit(1);
}

// Auxiliary functions enable to pass information about cube with given ID.
bool get_bit_state(Labyrinth lab, size_t cube_id) {
    size_t remainder = cube_id % NUMBER_OF_BITS_IN_BYTE;
    size_t i = cube_id / NUMBER_OF_BITS_IN_BYTE;
    size_t bits_number = lab->bits_array[i];
    return bits_number & (1 << remainder);
}

void set_bit_state(Labyrinth lab, size_t cube_id) {
    size_t remainder = cube_id % NUMBER_OF_BITS_IN_BYTE;
    size_t i = cube_id / NUMBER_OF_BITS_IN_BYTE;
    lab->bits_array[i] = lab->bits_array[i] | (1 << remainder);
}

// Function checks if there are characters other than whitespace at the end
// of the input line.
void wrong_input(Labyrinth lab, int line_number) {
    int c;
    while ((c = getchar()) && c != EOF && c != '\n') {
        if (!isspace(c)) {
            error(lab, line_number);
        }
    }
    if (line_number == 4) {
        while ((c = getchar()) && c != EOF) {
            if (c == '\n' || !isspace(c)) {
                error(lab, 5);
            }
        }
    }
}

// Function adds an element to the given array and resizes an array if needed.
void add_int(Labyrinth lab, size_t **array, size_t element,
             size_t *size, size_t *max_size) {
    (*array)[*size] = element;
    ++(*size);
    if (*size == *max_size - 1) {
        *max_size *= 2;
        *array = realloc(*array, *max_size * sizeof(size_t));
        if(*array == NULL) {
            error(lab, 0);
        }
    }
}

// Function reads a number from input.
size_t get_number(Labyrinth lab, int line_number) {
    size_t result_int = 0;
    char c;
    for (c = fgetc(stdin); c != EOF && !isspace(c); c = fgetc(stdin)) {
        if (c < '0' || c > '9')
            error(lab, line_number);
        int temp_int = c - '0';
        result_int = result_int * 10 + temp_int;
    }

    ungetc(c, stdin);
    return result_int;
}

void parse_1(Labyrinth lab) {
    size_t max_size = 16;

    char c;
    while ((c = getchar()) && c != EOF && c != '\n') {
        if (!isspace(c)) {
            ungetc(c, stdin);
            size_t x = get_number(lab, 1);

            // Safe multiplication considering possible overflow.
            if (__builtin_umulll_overflow(lab->size, x,
                                          (unsigned long long *)&(lab->size))) {
                error(lab, 0);
            }

            if (x < 1) {
                error(lab, 1);
            }

            add_int(lab, &(lab->dimensions_array), x,
                    &(lab->dimensions_number), &max_size);
        }
    }
}

size_t parse_2_3(Labyrinth lab, int line_number) {
    size_t i = 0;
    size_t cube_id = 0; // Index of the cube described with given coordinates.
    size_t multiplier = 1;  // Equals to n_1*n_2*...*n_{k-1} while adding
                            // k-coordinate.

    char c;
    while ((c = getchar()) && c != EOF && c != '\n') {
        if ('0' <= c && c <= '9') {
            ungetc(c, stdin);
            size_t x = get_number(lab, line_number);

            if (x == 0) {
                error(lab, line_number);
            }
            if (x > lab->dimensions_array[i]) {
                error(lab, line_number);
            }

            cube_id += (x - 1) * multiplier;
            multiplier *= lab->dimensions_array[i];
            i++;

            if (i > lab->dimensions_number) {
                error(lab, line_number);
            }
        }
        else if (!isspace(c)) {
            error(lab, line_number);
        }
    }
    // Checking if input size is correct.
    if (i != lab->dimensions_number) {
        error(lab, line_number);
    }

    return cube_id;
}

// Auxiliary functions help parse fourth line of input.
// Location of labyrinth walls is stored in [lab->bits_array],
// each unsigned_char describes 8 cubes.

char get_decimal(Labyrinth lab, char c) {
    if ('0' <= c && c <= '9')
        return c - '0';
    else if ('a' <= c && c <= 'f')
        return c - 'a' + 10;
    else if ('A' <= c && c <= 'F')
        return c - 'A' + 10;
    else
        error(lab, 4);
}

// Function returns the smallest integral value that is not less
// than a / b.
size_t ceiling(size_t a, size_t b) {
    size_t remainder = a % b;
    if (remainder > 0)
        return a / b + 1;
    else
        return a / b;
}

// Function checks if all the marked cubes are within lab->size.
bool check_size(Labyrinth lab) {
    size_t string_size = lab->bits_number * NUMBER_OF_BITS_IN_BYTE;
    for (size_t i = lab->size; i < string_size; i++) {
        if (get_bit_state(lab, i)) {
            error(lab, 4);
        }
    }
}

// Function corrects the issues with merging two hexadecimal numbers
// into one cell of [lab->bits_array] caused by lack of information
// about input size while filling the array.
void fix_bits_array(Labyrinth lab, size_t input_size) {
    for (size_t i = 0; i < input_size; i++) {
        (lab->bits_array)[i] /= BASE;
        if (i != input_size - 1) {
            unsigned char fold = (lab->bits_array)[i + 1] % BASE;
            (lab->bits_array)[i] += fold * BASE;
        }
    }
}

void too_small_input(Labyrinth lab, size_t input_size) {
    size_t difference = lab->bits_number - input_size;
    for (size_t i = 0; i < lab->bits_number; i++) {
        if (i < input_size) {
            (lab->bits_array)[i] = (lab->bits_array)[i + difference];
        }
        else {
            (lab->bits_array)[i] = 0;
        }
    }
}

// Function reads hexadecimal number from fourth line of input and converts it
// to fit in [lab->bits_array] - two hexadecimal numbers are merged in one cell.
void parse_4a(Labyrinth lab) {
    getchar();
    char c, x;
    size_t input_size = 0;
    bool first = true; // Merging starts from the most significant bit.
    bool input_too_big = false;

    while ((c = getchar()) && c != EOF && c != '\n' && !isspace(c)) {
        if (input_too_big)
            error(lab, 4);
        if (input_size != 0 || c != '0') {
            x = get_decimal(lab, c);
            // The first element of input is stored in the last cell of an array.
            if (first) {
                (lab->bits_array)[lab->bits_number - input_size - 1] = x * BASE;
                first = false;
                input_size++;
            }
            else {
                (lab->bits_array)[lab->bits_number - input_size] += x;
                first = true;
            }
        }
        if (input_size > lab->bits_number)
            input_too_big = true;
    }

    // Since the array was pre-allocated, if the input length divided by two
    // is smaller than [lab->bits_number], array has to be reordered so that
    // information about the least significant bit is in the first cell of array.
    if (input_size < lab->bits_number)
        too_small_input(lab, input_size);

    if (!first)
        fix_bits_array(lab, input_size);

    check_size(lab);

    ungetc(c, stdin);
    wrong_input(lab, 4);
}

// Auxiliary function to separate a number after character 'R' in fourth line
// of input.
size_t get_number_without_white_space(Labyrinth lab) {
    char c;
    while ((c = getchar()) && c != '\n' && isspace(c)) {}
    ungetc(c, stdin);
    if (c == '\n' || c == EOF) {
        error(lab, 4);
    }
    size_t result = get_number(lab, 4);
    if (result > UINT32_MAX) {
        error(lab, 4);
    }
    return result;
}

// Function marks as walls cubes with IDs calculated from numbers given
// in fourth line of input after the 'R' character.
void parse_4b(Labyrinth lab) {
    size_t a, b, m, r, s;

    a = get_number_without_white_space(lab);
    b = get_number_without_white_space(lab);
    m = get_number_without_white_space(lab);
    if (m == '0')
        error(lab, 4);
    r = get_number_without_white_space(lab);
    s = get_number_without_white_space(lab);

    wrong_input(lab, 4);

    size_t w, previous_s = s;

    // Calculating and marking cubes that are walls in [lab->bits_array].
    for (size_t i = 1; i <= r; i++) {
        s = a * previous_s + b;
        s %= m;
        w = s % lab->size;
        set_bit_state(lab, w);
        w += TWO_POWER_32;
        while (w < lab->size) {
            set_bit_state(lab, w);
            w += TWO_POWER_32;
        }
        previous_s = s;
    }
}

// Function reads the beginning of fourth line of input and checks
// which case it describes. Otherwise, it returns ERROR 4.
void parse_4(Labyrinth lab) {
    char c;
    while ((c = getchar()) && c != EOF) {
        if (c == '\n')
            error(lab, 4);
        if (!isspace(c)) {
            if (c == 'R')
                parse_4b(lab);
            else if (c == '0')
                parse_4a(lab);
            else
                error(lab, 4);
            break;
        }
    }
}