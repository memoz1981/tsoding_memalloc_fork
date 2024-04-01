#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "./heap.h"
#include "./linked_list.h"

uintptr_t heap[HEAP_CAP_WORDS] = {0};
const uintptr_t *stack_base = 0;

