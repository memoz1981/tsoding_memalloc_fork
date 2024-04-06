CFLAGS=-Wall -Wextra -Werror -std=c11 -pedantic -ggdb

# Default target
all: clean heap

# Target for building heap with heap1.c
chunk: main.c heap_using_chunk_lists.c heap.h chunk_lists.h
	$(CC) $(CFLAGS) -o heap main.c heap_using_chunk_lists.c logger.c

# Target for building heap with heap2.c
linkedlist: main.c heap_using_linked_lists.c heap.h logger.c logger.h linked_list.h
	$(CC) $(CFLAGS) -o heap main.c heap_using_linked_lists.c logger.c

# Target for cleaning up
clean:
	rm -f heap

.PHONY: all heap1 heap2 clean
