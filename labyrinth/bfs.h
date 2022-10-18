#ifndef BFS_H
#define BFS_H

// Function implements a breadth-first search algorithm.
// Returns true if a way was found.
bool bfs(Labyrinth lab, size_t token, size_t finish, size_t *distance);

#endif