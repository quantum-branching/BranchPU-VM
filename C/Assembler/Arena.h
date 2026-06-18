#include <stdlib.h>
#define PAGE_SIZE 4096

struct Arena {
	void *ptr;
    int size;
    int current;
	struct Arena *next;
};

struct Arena *arena_new(int size);

void *arena_allocate(struct Arena *arena, int size);

void arena_free(struct Arena *arena);