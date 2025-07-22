#include "memoryarena.h"

MemArena* create_arena(size_t size, size_t align) {
	MemArena* arena = (MemArena*)malloc(sizeof(MemArena));

	assert(arena != NULL);

	arena->base = malloc(size);
	arena->size = size;
	arena->offset = 0;
	arena->align = align;

	return arena;
}

void* enter_the_arena(MemArena* arena, size_t size) {

	size_t aligned_offset = (arena->offset + (arena->align - 1)) & ~(arena->align - 1);

	if (aligned_offset + size > arena->size) 
		return NULL;

	arena->offset = aligned_offset + size;

	return (char*)arena->base + aligned_offset;
}

void destroy_arena(MemArena* arena) {
	free(arena->base);
	free(arena);
}