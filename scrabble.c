#include <string.h>
#include <ctype.h>
#include "hashtable.h"

/*list of primes for the hashing function*/
static const int primes[26] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43,
			47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101};

struct combo {
	int *combo;
	int *reset;
	char *str;
	int length;
	size_t max;
};

static unsigned char normalize_letter(unsigned char c) {
	if (islower(c))
		return c - 97;
	else
		return c - 65;
}

static void fill_buckets(char const *str, int *bucket) {
	size_t index;
	size_t length = strlen(str);

	for (index=0; index < length; index++) {
		bucket[normalize_letter(str[index])] += 1;
	}
}

static struct combo *create_combo_array(char const *letters) {
	int bucket[26];
	int *combo;
	int *reset;
	unsigned int next = 0;
	unsigned int index;
	size_t len = strlen(letters);
	char *str = (char *) ecalloc(len + 1, sizeof(char));
	struct combo *result = (struct combo *) emalloc(sizeof(struct combo));

	memset(bucket, 0, 26 * sizeof(int));
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
	result->max = len;

	return result;
}

static size_t word_hasher(char const *str) {
	size_t result = 1;
	size_t index;
	size_t length = strlen(str);

	/*creating hash*/
	for (index=0; index < length; index++) {
		result *= primes[normalize_letter(str[index])];
	}
	return result;

}

static char *sanitizer(char const *word) {
	size_t length = strlen(word);
	size_t current = 0;
	size_t index;
	char *result = (char *) emalloc(length + 1);

	for (index=0; index < length; index++) {
		if (isalpha(word[index])) {
			result[current] = word[index];
			current++;
		}
	}
	result[current] = '\0';
	return result;
}

static hashtable *initialize(FILE *list) {
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
		exit(EXIT_FAILURE);
	}

	rewind(list);
	word_set = init_table(lines * 2);

	/*getting words from word list*/
	while ((fgets(word, 100, list) != NULL)) {
		str = sanitizer(word);
		insert(word_set, word_hasher(str), str);
	}

	printf("Number of words: %lu\n", (unsigned long) lines);
	return word_set;
}

static int is_anagram(char const *str, char const *other) {
	/*creating buckets to bucket sort (array of zeroes)*/
	int bucket[26];
	int other_bucket[26] = { 0 };
	unsigned int index;

	memset(bucket, 0, 26 * sizeof(int));

	fill_buckets(str, bucket);
	fill_buckets(other, other_bucket);

	/*comparing buckets*/
	for (index=0; index < 26; index++) {
		if (bucket[index] != other_bucket[index])
			return 0;
	}
	return 1;
}

static int word_searcher(hashtable *word_set, char *word, hashtable *output) {
	/*output is to sort by length*/
	llist *buffer;
	int result = 0;
	char *copy;

	/*finding word with the same hashes*/
	if ((buffer = get_bucket(word_set, word_hasher(word))) != NULL) {
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

static char *word_subset(struct combo *combination) {
	int i;
	int j;
	int current = 0;
	char *result;

	result = (char *) emalloc(sizeof(int) * combination->max+1);


	for (i=0; i < combination->length; i++)  {
		for (j=0; j < combination->combo[i]; j++) {
			result[current] = combination->str[i];
			current++;
		}
	}
	result[current] = '\0';
	return result;
}

static int decrement(struct combo *combination) {
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

static void scrabbler(hashtable *word_set, char *letters) {
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
		exit(EXIT_FAILURE);
	}

	if (argc == 1) {
		if ((list = fopen("/usr/share/dict/words", "r")) == NULL) {
			perror("fopen");
			fprintf(stderr, "Unix word list not found.  Please "
					"give path to word list as argument.\n");
			exit(EXIT_FAILURE);
		}
	} else if ((list = fopen(argv[1], "r")) == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
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
