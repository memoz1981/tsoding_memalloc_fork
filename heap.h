#ifndef HEAP_H_
#define HEAP_H_

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#define HEAP_CAP_BYTES 640000
static_assert(HEAP_CAP_BYTES % sizeof(uintptr_t) == 0,
              "The heap capacity is not divisible by "
              "the size of the pointer. Of the platform.");
#define HEAP_CAP_WORDS (HEAP_CAP_BYTES / sizeof(uintptr_t))

extern uintptr_t heap[HEAP_CAP_WORDS];
extern const uintptr_t *stack_base;

void *heap_alloc(size_t size_bytes);
void heap_free(void *ptr);
void heap_collect();

#endif // HEAP_H_