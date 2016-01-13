#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include "treapset.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int word_hasher(char *str) {
	/*creating buckets to bucket sort (array of ones)*/
	int bucket[26] = { 1 };
	int result = 0;
	unsigned int index;
	/*filling buckets*/
	for (index=0; index < strlen(str); index++) {
		if ((str[index] >= 97) && (str[index] <= 122))
			bucket[str[index] - 97] += 1;

		if ((str[index] >= 65) && (str[index] <= 90))
			bucket[str[index] - 65] += 1;
	}

	/*creating hash*/
	for (index=0; index < 26; index++) {
		result += bucket[index] * (index+1);
	}
	return result;

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
	/*creating buckets to bucket sort (array of zeroes)*/
	int bucket[26] = { 0 };
	int other_bucket[26] = { 0 };
	
	unsigned int index;

	/*the buckets keep track of the number of each letter in each string*/
	for (index=0; index < strlen(str); index++) {
		if ((str[index] >= 97) && (str[index] <= 122))
			bucket[str[index] - 97] += 1;

		if ((str[index] >= 65) && (str[index] <= 90))
			bucket[str[index] - 65] += 1;
	}
	for (index=0; index < strlen(other); index++) {
		if ((other[index] >= 97) && (other[index] <= 122))
			other_bucket[other[index] - 97] += 1;

		if ((other[index] >= 65) && (other[index] <= 90))
			other_bucket[other[index] - 65] += 1;
	}

	/*comparing buckets*/
	for (index=0; index < 26; index++) {
		if (bucket[index] != other_bucket[index])
			return 0;
	}
	return 1;


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
		if ((list = fopen("/usr/share/dict/words", "r")) == NULL) {
			perror("fopen");
			fprintf(stderr, "Unix word list not found.  Please give path to word list as argument.\n");
			exit(1);
		}
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
