#include "hashtable.h"

void *emalloc(size_t size) {
		void *result;
		if ((result = malloc(size)) == NULL) {
				perror("malloc");
				exit(1);
		}
		return result;
}

void *ecalloc(size_t nmemb, size_t size) {
		void *result;
		if ((result = calloc(nmemb, size)) == NULL) {
				perror("calloc");
				exit(1);
		}
		return result;
}

hashtable *init_table(size_t size) {
	hashtable *result = (hashtable *) emalloc(sizeof(hashtable));
	result->size = size;
	result->table = (llist **) ecalloc(size, sizeof(llist *));
	return result;
}

void insert(hashtable *table, size_t hash, char *str) {
	llist *new_node = (llist *) emalloc(sizeof(llist));

	new_node->data = str;
	new_node->next = table->table[hash % (table->size)];

	table->table[hash % (table->size)] = new_node;
}

void destroy_list(llist *list) {
		llist *temp;
		while (list != NULL) {
				temp = list->next;
				free(list->data);
				free(list);
				list = temp;
		}
}

void destroy_table(hashtable *table) {
		size_t i;
		for (i = 0; i < table->size; i++) 
				destroy_list(table->table[i]);
		free(table->table);
		free(table);
}

void print_list(llist const *list) {
		while (list != NULL) {
				printf("%s\n", list->data);
				list = list->next;
		}
}

void print_table(hashtable const *table) {
		size_t index;
		
		for (index = 0; index < table->size; index++) {
				print_list(table->table[index]);
		}
}
