#include "treapset.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*list of primes for the hashing function*/
const int primes[26] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 32, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101};

void *emalloc(size_t bytes) {
	void *result;
	if ((result = malloc(bytes)) == NULL) {
		fprintf(stderr, "Out of memory!\n");
		exit(1);
	}
	return result;
}

unsigned long power(int x, int y) {
	unsigned long result = (unsigned long) x;

	if (y == 0) {
		return 1;
	}
	for (int i=1; i < y; i++) {
		result *= x;
	}
	return result;
}

unsigned long word_hasher(char *str) {
	/*creating buckets to bucket sort (array of ones)*/
	int bucket[26] = { 0 };
	unsigned long result = 1;
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
		result *= power(primes[index], bucket[index]);
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
		str = (char *) emalloc(strlen(word) + 1);
		strcpy(str, word);
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
				printf("%s", buffer->str);
			/*searching for more words with the same hash using middle node*/
			buffer = buffer->middle;
		}
	}
}

char *word_subset(char *letters, int *combination) {
	int length = strlen(letters) - 1;
	char *result = (char *) emalloc(sizeof(int) * length+1);
	int current = 0;
	for (int i=0; i < length; i++)  {
		if (combination[i] == 1) {
			result[current] = letters[i];
			current++;
		}
	}
	result[current] = '\0';
	return result;
}

void decrement(int *combination, int length) {
	for (int i=length-1; i >= 0; i--) {
		if (combination[i] == 0) {
			combination[i] = 1;
		} else {
			combination[i] = 0;
			i = -1;
		}
	}
}

void scrabbler(treapset *word_set, char *letters) {
	int length = strlen(letters) - 1;
	int *combination = (int *) emalloc(length * sizeof(int));
	for (int i=0; i < length; i++)
		combination[i] = 1;

	int number_combinations = (int) power(2, length) - 1;
	char *word;
	for (int i=0; i < number_combinations; i++) {
		word = word_subset(letters, combination);
		word_searcher(word_set, word);
		free(word);
		decrement(combination, length);
	}
	free(combination);

}

char *sanitizer(char *word) {
	int length = strlen(word);
	char *result = (char *) emalloc(length);
	memset(result, 0, sizeof(char)*length);
	for (int i=0; i < length; i++) {
		if ((word[i] <= 122) && (word[i] >= 97)) {
			result[i] = word[i];
		} else if ((word[i] <= 90) && (word[i] >= 65)) {
			result[i] = word[i];
		}
	}
	return result;
}

int main(int argc, char **argv) {
	FILE *list;
	treapset *word_set;
	char input_buffer[100];

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
	while (fgets(input_buffer, 100, stdin) != NULL) {
		printf("\n");
		char *word = sanitizer(input_buffer);
		scrabbler(word_set, word);
		free(word);
		printf("\n");
	}

	printf("\nFreeing memory...\n");
	destroy_treap(word_set);
	printf("Done!\n");
	return 0;
}
