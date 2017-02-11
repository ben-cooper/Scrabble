#include <string.h>
#include <ctype.h>
#include "hashtable.h"

/*list of primes for the hashing function*/
const int primes[26] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43,
			47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101};

struct combo {
	int *combo;
	int *reset;
	char *str;
	int length;
};

int normalize_letter(int c) {
		if (islower(c))
				return c - 97;
		else
				return c - 65;
}

void fill_buckets(char const *str, int *bucket) {
	unsigned int index;
	for (index=0; index < strlen(str); index++) {
			bucket[normalize_letter(str[index])] += 1;
	}
}

struct combo *create_combo_array(char const *letters) {
	int bucket[26] = { 0 };
	int *combo;
	int *reset;
	struct combo *result = (struct combo *) emalloc(sizeof(struct combo));
	unsigned int next = 0;
	unsigned int index;
	char *str = (char *) ecalloc(strlen(letters) + 1, sizeof(char));

	fill_buckets(letters, bucket);

	/* removing duplicate letters from input */
	for (index=0; index < 26; index++) {
		if (bucket[index] != 0) {
			str[next] = index + 97;
			next += 1;
		}
	}


	/* creating resulting combo array */
	combo = (int *) emalloc(next * sizeof(int));
	reset = (int *) emalloc(next * sizeof(int));


	next = 0;
	for (index=0; index < 26; index++) {
		if (bucket[index] != 0) {
			combo[next] = bucket[index];
			reset[next] = bucket[index];
			next += 1;
		}
	}

	result->combo = combo;
	result->reset = reset;
	result->length = next;
	result->str = str;

	return result;
}

unsigned long word_hasher(char const *str) {
	unsigned long result = 1;
	unsigned int index;

	/*creating hash*/
	for (index=0; index < strlen(str); index++) {
			result *= primes[normalize_letter(str[index])];
	}
	return result;

}

char *sanitizer(char const *word) {
	int length = strlen(word);
	int current = 0;
	int i;
	char *result = (char *) emalloc(length);

	for (i=0; i < length - 1; i++) {
		if (isalpha(word[i])) {
			result[current] = word[i];
			current++;
		}
	}
	result[current] = '\0';
	return result;
}

hashtable *initialize(FILE *list) {
	hashtable *word_set;
	char word[100];
	char *str;
	size_t lines = 0;
	
	/*counting number of words in dictionary*/
	while ((fgets(word, 100, list) != NULL)) {
		lines += 1;
	}
	
	/* if empty file */
	if (lines == 0) {
		fprintf(stderr, "Initialization failed! Empty File.\n");
		exit(1);
	}

	rewind(list);
	word_set = init_table(lines * 2);

	/*getting words from word list*/
	while ((fgets(word, 100, list) != NULL)) {
		str = sanitizer(word);
		insert(word_set, word_hasher(str), str);
	}

	printf("Number of words: %zu\n", lines);
	return word_set;
}

int is_anagram(char const *str, char const *other) {
	/*creating buckets to bucket sort (array of zeroes)*/
	int bucket[26] = { 0 };
	int other_bucket[26] = { 0 };
	unsigned int index;

	fill_buckets(str, bucket);
	fill_buckets(other, other_bucket);

	/*comparing buckets*/
	for (index=0; index < 26; index++) {
		if (bucket[index] != other_bucket[index])
			return 0;
	}
	return 1;
}

int word_searcher(hashtable *word_set, char *word, hashtable *output) {
	/*output is to sort by length*/
	llist *buffer;
	int result = 0;
	char *copy;

	/*finding word with the same hashes*/
	if ((buffer = word_set->table[word_hasher(word) % (word_set->size)]) != NULL) {
		while (buffer != NULL) {
			/*seeing if words are actually anagrams of each 
			 * other*/
			if (is_anagram(buffer->data, word) == 1) {
				copy = (char *) emalloc (strlen(buffer->data) + 1);
				strcpy(copy, buffer->data);
				insert(output, strlen(word) - 1, copy);
				result = 1;
			}
			/*checking other words in the linked list*/
			buffer = buffer->next;
		}
	}

	return result;
}

char *word_subset(struct combo *combination) {
	int i;
	int j;
	int current = 0;
	char *result;

	result = (char *) emalloc(sizeof(int) * combination->length+1);


	for (i=0; i < combination->length; i++)  {
		for (j=0; j < combination->combo[i]; j++) {
			result[current] = combination->str[i];
			current++;
		}
	}
	result[current] = '\0';
	return result;
}

int decrement(struct combo *combination) {
	int i;
	int ret=1;
	for (i=combination->length-1; i >= 0; i--) {
		if (combination->combo[i] == 0) {
			combination->combo[i] = combination->reset[i];
		} else {
			combination->combo[i] -= 1;
			ret = 0;
			i = -1;
		}
	}
	return ret;
}

void scrabbler(hashtable *word_set, char *letters) {
	struct combo *combination;
	int stop=0;
	char *word;
	hashtable *output = init_table(strlen(letters));

	combination = create_combo_array(letters);

	while (stop == 0) {
		word = word_subset(combination);
		word_searcher(word_set, word, output);
		free(word);
		stop = decrement(combination);
	}

	free(combination->combo);
	free(combination->reset);
	free(combination->str);
	free(combination);
	print_table(output);
	destroy_table(output);
}

int main(int argc, char **argv) {
	FILE *list;
	hashtable *word_set;
	char input_buffer[100];
	char *word;

	if (argc > 2) {
		fprintf(stderr, "usage: %s [path_to_word_list]\n", argv[0]);
		exit(1);
	}

	if (argc == 1) {
		if ((list = fopen("/usr/share/dict/words", "r")) == NULL) {
			perror("fopen");
			fprintf(stderr, "Unix word list not found.  Please \
				give path to word list as argument.\n");
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
		word = sanitizer(input_buffer);
		scrabbler(word_set, word);
		free(word);
		printf("\n");
	}

	printf("\nFreeing memory...\n");
	destroy_table(word_set);
	printf("Done!\n");
	return 0;
}
