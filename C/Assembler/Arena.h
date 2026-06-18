#include <stdlib.h>
#define PAGE_SIZE 4096

struct Arena {
	void *ptr;
    int size;
    int current;
	struct Arena *next;
};

/// @brief Creates a new arena of a given size.
/// @param size The size of the arena to create.
/// @return A pointer to the new arena.
struct Arena *arena_new(int size);

/// @brief Allocates memory from a given arena.
/// @param arena The arena to allocate from.
/// @param size The size of the memory to allocate.
/// @return A pointer to the allocated memory.
void *arena_allocate(struct Arena *arena, int size);

/// @brief Sets all bytes in a given arena to zero.
/// @param arena The arena to empty.
void arena_empty(struct Arena *arena);

/// @brief Frees the memory used by an arena.
/// @param arena The arena to free.
void arena_free(struct Arena *arena);