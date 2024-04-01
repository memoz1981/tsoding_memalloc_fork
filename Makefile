CFLAGS=-Wall -Wextra -Werror -std=c11 -pedantic -ggdb

heap: main.c heap_using_linked_lists.c heap.h
	$(CC) $(CFLAGS) -o heap main.c heap_using_linked_lists.c
