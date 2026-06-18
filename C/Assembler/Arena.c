#include "Arena.h"

#define debug(x,y) //printf(x,y)

struct Arena *currentArena;

struct Arena *arena_new(int size) {
	struct Arena *result = malloc(sizeof(struct Arena));
	result->ptr = malloc(size);
	result->size = size;
	result->current = 0;
	result->next = NULL;
	return result;
}

void *arena_allocate(struct Arena *arena, int size) {
	if(arena == NULL) {
		printf("Error: Arena is NULL\n");
		return malloc(size);
	}
	
	if(arena->current + size > arena->size) {
		if(arena->next != NULL) {
			return arena_allocate(arena->next, size);
		}

		arena->next = arena_new(arena->size);
		arena->next->current = size;
		return arena->next->ptr;
	}

	void *ptr = arena->ptr + arena->current;
	arena->current += size;
	return ptr;
}

void arena_empty(struct Arena *arena) {
	void *base = arena->ptr;
	for(int i = 0; i < arena->size; i++) {
		*((char*)base + i) = 0;
	}
}

void arena_free(struct Arena *arena) {
	if(arena->next != NULL) {
		arena_free(arena->next);
	}
	"";
	debug("Freeing %d bytes\n", arena->current);
	free(arena->ptr);
	free(arena);
}

#define free(x)
#define enterArena(x) currentArena = arena_new(x)
#define malloc(x) arena_allocate(currentArena, x)
#define exitArena() arena_free(currentArena)
#define realloc(x, y) arena_allocate(currentArena, y)