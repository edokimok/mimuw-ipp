#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.h"
#include "parse_input.h"
#include "bfs.h"

// Function calculates IDs of cubes neighbouring to [cube]
// and adds them to the [lab->queue].
void add_adjacent_cubes(Labyrinth lab, size_t cube) {
    size_t dimensions = 1;
    // While calculating neighbouring cubes IDs, function checks if [cube] has
    // enough space for two adjacent cubes in both directions in each dimension.
    // If so, the results of cubes' ID's divided by the product of consecutive
    // dimensions match.
    size_t divider;

    for (size_t i = 0; i < get_dimensions_number(lab); i++) {
        if (cube < get_size(lab) - dimensions) {
            size_t new_cube1 = cube + dimensions;
            if (!get_bit_state(lab, new_cube1)) {
                divider = dimensions * read_dimensions_array(lab, i);
                if (new_cube1 / divider == cube / divider) {
                    push(get_queue(lab), new_cube1);
                    set_bit_state(lab, new_cube1);
                }
            }
        }
        if (cube >= dimensions) {
            size_t new_cube2 = cube - dimensions;
            if (!get_bit_state(lab, new_cube2)) {
                divider = dimensions * read_dimensions_array(lab, i);
                if (new_cube2 / divider == cube / divider) {
                    push(get_queue(lab), new_cube2);
                    set_bit_state(lab, new_cube2);
                }
            }
        }
        dimensions *= read_dimensions_array(lab, i);
    }
}

// Length of the shortest path to [finish] cube is stored in [distance].
bool bfs(Labyrinth lab, size_t token, size_t finish, size_t *distance) {
    bool found = false;
    while (!empty(get_queue(lab))) {
        size_t cube = front(get_queue(lab));
        // Token at the front of the queue increments distance to the finish
        // cube. Then the token is pushed back at the end of the queue.
        if (cube == token && last(get_queue(lab)) != token) {
            (*distance)++;
            pop(get_queue(lab));
            push(get_queue(lab), token);
        }
        else {
            if (cube == finish) {
                found = true;
                break;
            }
            add_adjacent_cubes(lab, cube);
            if (cube == token && last(get_queue(lab)) != token) {
                push(get_queue(lab), token);
                (*distance)++;
            }
            pop(get_queue(lab));
        }
    }
    return found;
}