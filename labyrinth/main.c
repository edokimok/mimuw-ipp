#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.h"
#include "parse_input.h"
#include "bfs.h"

int main() {
    Labyrinth lab = create_labyrinth();

    parse_1(lab);

    // A size of bits_array is equal to the number of the cubes
    // divided by number of bits in char.
    set_bits_number(lab, ceiling(get_size(lab), 8));
    create_bits_array(lab);

    // Checking if arrays were allocated correctly.
    if (get_dimensions_array(lab) == NULL || get_bits_array(lab) == NULL)
        error(lab, 0);

    size_t start = parse_2_3(lab, 2);
    size_t finish = parse_2_3(lab, 3);

    parse_4(lab);

    // Prints error if cube of starting or finishing position is not free.
    if (get_bit_state(lab, start))
        error(lab, 2);
    if (get_bit_state(lab, finish))
        error(lab, 3);

    push(get_queue(lab), start);
    set_bit_state(lab, start);

    size_t distance = 0;
    if (bfs(lab, start, finish, &distance))
        printf("%lu\n", distance);
    else
        printf("NO WAY\n");

    free_all(lab);

    return 0;
}