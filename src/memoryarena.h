#pragma once

#include <windows.h>
#include <assert.h>

#define KB_4 1<<12
#define MB_16 1<<24
#define DEF_ALIGN 8

typedef struct {
	void* base;
	size_t size;
	size_t offset;
	size_t align;

} MemArena;

MemArena* create_arena(size_t size, size_t align);
void* enter_the_arena(MemArena* arena, size_t size);
void destroy_arena(MemArena* arena);