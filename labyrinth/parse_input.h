#ifndef PARSE_INPUT_H
#define PARSE_INPUT_H

// Structure stores description of the labyrinth:
// [dimensions_array] with its length [dimensions_number],
// number of cubes [size], pointer to the queue needed to
// search for the shortest way from start to finish
// and an array [bits_array] that contains
// information about location of the walls as unique indexed
// calculated with given function.
typedef struct Labyrinth *Labyrinth;

// Set of functions that allow passing structure's members.
size_t *get_dimensions_array(Labyrinth lab);
size_t read_dimensions_array(Labyrinth lab, size_t index);
size_t get_dimensions_number(Labyrinth lab);
size_t get_size(Labyrinth lab);
size_t get_bits_number(Labyrinth lab);
void set_bits_number(Labyrinth lab, size_t value);
unsigned char *get_bits_array(Labyrinth lab);
void create_bits_array(Labyrinth lab);
Queue get_queue(Labyrinth lab);
void free_all(Labyrinth lab);

// Function creates an empty labyrinth.
Labyrinth create_labyrinth();

// Function frees allocated memory and terminates the program.
void error(Labyrinth lab, int error_number);

// Function returns false if the specified cube is free
// and true if it's a wall.
bool get_bit_state(Labyrinth lab, size_t number);

// Function marks specified cube in [lab->bits_array] as not free.
void set_bit_state(Labyrinth lab, size_t number);

// Function reads the first line of input and fills [lab->dimensions_array].
void parse_1(Labyrinth lab);

// Function reads second and third line of input and calculates
// starting and finishing position.
size_t parse_2_3(Labyrinth lab, int line_number);

// Function returns the smallest integral value that is not less
// than a divided by b.
size_t ceiling(size_t a, size_t b);

// Function reads fourth line of input and marks specified cubes as walls.
void parse_4(Labyrinth lab);

#endif