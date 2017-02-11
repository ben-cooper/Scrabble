#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

typedef struct llist {
		char *data;
		struct llist *next;
} llist;

typedef struct hashtable {
	size_t size;
	llist **table;
} hashtable;

extern void *emalloc(size_t size);
extern void *ecalloc(size_t nmemb, size_t size);
extern hashtable *init_table(size_t size);
extern void insert(hashtable *table, size_t hash, char *str);
extern void destroy_list(llist *list);
extern void destroy_table(hashtable *table);
extern void print_list(llist const *list);
extern void print_table(hashtable const *table);
