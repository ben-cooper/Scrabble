#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include "treapset.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

unsigned char *word_sorter(unsigned char *str) {
	int bucket[26] = {  0  };
	int length = strlen((char *) str);
	unsigned char *result;
	int index = 0;
	/*loading buckets*/
	for (int i=0; i < length; i++){
		int chr = (int) str[i];
		if ((chr >= 97) && (chr <= 122)) {
			bucket[chr-97] += 1;
		} else if ((chr >= 65) && (chr <= 90)) {
			bucket[chr-65] += 1;
		}
	}
	/*unloading buckets into a string*/
	if ((result = (unsigned char *) malloc(sizeof(char) * (length + 1))) == NULL) {
		fprintf(stderr, "Out of memory!\n");
		exit(1);
	}
	for (int i=0; i < 26; i++) {
		for (int j=0; j < bucket[i]; j++) {
			result[index] = (unsigned char) (i+97);
			index++;
		}
	}

	result[length+1] = '\0';
	return result;

}

unsigned long word_hasher(char *string) {
	
	unsigned char *raw = (unsigned char *) string;
	unsigned char *str = word_sorter(raw);
	unsigned char *copy = str;
	unsigned long hash = 5381;
	int c;

	while ((c = *str++) != 0)
        	hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	free(copy);
	return hash;
}

treapset *initialize(FILE *list) {
	/*making treap*/
	treapset *word_set = NULL;

	/*making buffer*/
	char word[100];

	/*allocating new string*/
	char *str;

	/*seeding the treap*/
	seed_rand();


	/*getting words from word list*/
	while ((fgets(word, 100, list) != NULL)) {
		word[strlen(word) - 1] = '\0';
		if ((str = (char *) malloc(strlen(word))) == NULL) {
			fprintf(stderr, "Out of memory!\n");
			exit(1);
		}
		strcat(str, word);
		word_set = treap_insert(word_set, word_hasher(str), str);
	}

	printf("Number of words: %d\n", treap_length(word_set));

	printf("Height: %d\n", treap_height(word_set));

	return word_set;



}
int word_compare(char *str, char *other) {
	unsigned char *uother = (unsigned char *) other;
	unsigned char *ustr = (unsigned char *) str;
	if (strcmp((char *)word_sorter(uother), (char *)word_sorter(ustr)) == 0)
		return 1;
	return 0;

}

void word_searcher(treapset *word_set, char *word) {

	treapset *buffer;

	/*finding word with the same hashes*/
	if ((buffer = treap_find(word_set, word_hasher(word))) != NULL) {
		while (buffer != NULL) {
			/*seeing if words are actually anagrams of each other*/
			if (word_compare(buffer->str, word) == 1)
				printf("%s\n", buffer->str);
			/*searching for more words with the same hash using middle node*/
			buffer = buffer->middle;
		}
	} else {
		printf("Not found!\n");
	}
}




int main(int argc, char **argv) {
	FILE *list;
	treapset *word_set;
	char *input_buffer = NULL;
	unsigned long len;
	int read;
	
	if (argc > 2) {
		fprintf(stderr, "usage: %s [path_to_word_list]\n", argv[0]);
		exit(1);
	}

	if (argc == 1) {
		list = fopen("/usr/share/dict/words", "r");
	} else if ((list = fopen(argv[1], "r")) == NULL) {
		perror("fopen");
		exit(1);
	}

	/*getting word set*/
	printf("Initializing word list...\n");
	word_set = initialize(list);
	fclose(list);
	printf("Done!\n\n");

	/*user input loop*/
	while ((read = getline(&input_buffer, &len, stdin)) != -1)  {
		input_buffer[strlen(input_buffer)-1] = '\0';
		printf("\n");
		word_searcher(word_set, input_buffer);
		printf("\n");
	}
	printf("\nFreeing memory...\n");
	destroy_treap(word_set);
	printf("Done!\n");
	return 0;
}
