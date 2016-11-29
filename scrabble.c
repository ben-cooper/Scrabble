#include "treapset.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*list of primes for the hashing function*/
const int primes[26] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 32, 37, 41, 43,
			47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101};

treapset *output = NULL;

void *emalloc(size_t bytes) {
	void *result;
	if ((result = malloc(bytes)) == NULL) {
		fprintf(stderr, "Out of memory!\n");
		exit(1);
	}
	return result;
}

void fill_buckets(char *str, int *bucket) {
	unsigned int index;
	for (index=0; index < strlen(str); index++) {
		if ((str[index] >= 97) && (str[index] <= 122))
			bucket[str[index] - 97] += 1;

		if ((str[index] >= 65) && (str[index] <= 90))
			bucket[str[index] - 65] += 1;
	}
}

int *create_combo_array(char *str, int *unique) {
	int bucket[26] = { 0 };
	int *result;
	unsigned int length = 0;
	unsigned int next = 0;
	unsigned int index;

	fill_buckets(str, bucket);

	/* counting filled buckets */
	for (index=0; index < 26; index++) {
		if (bucket[index] != 0)
			length += 1;
	}

	/* removing duplicate letters from input */
	for (index=0; index < 26; index++) {
		if (bucket[index] != 0) {
			str[next] = index + 97;
			next += 1;
		}
	}
	str[next] = '\0';


	/* creating resulting combo array */
	result = (int *) emalloc(length * sizeof(int));

	next = 0;
	for (index=0; index < 26; index++) {
		if (bucket[index] != 0) {
			result[next] = bucket[index];
			next += 1;
		}
	}

	/* unique will be the length of result */
	*unique = length;

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
	fill_buckets(str, bucket);

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
	
	fill_buckets(str, bucket);
	fill_buckets(other, other_bucket);

	/*comparing buckets*/
	for (index=0; index < 26; index++) {
		if (bucket[index] != other_bucket[index])
			return 0;
	}
	return 1;


}

int word_searcher(treapset *word_set, char *word) {

	treapset *buffer;
	int result = 0;

	/*finding word with the same hashes*/
	if ((buffer = treap_find(word_set, word_hasher(word))) != NULL) {
		while (buffer != NULL) {
			/*seeing if words are actually anagrams of each 
			 * other*/
			if (word_compare(buffer->str, word) == 1) {
				output = treap_insert(output, 
						strlen(buffer->str),
						buffer->str);
				result = 1;
			}
			/*searching for more words with the same hash using 
			 * middle node*/
			buffer = buffer->middle;
		}
	}

	return result;
}

char *word_subset(char *letters, int *combination) {
	unsigned int length = 0;
	unsigned int i;
	int current = 0;
	char *result;

	/* calculating length */
	for (i=0; i < strlen(letters); i++) {
		length += combination[i];
	}

	result = (char *) emalloc(sizeof(int) * length+1);


	for (i=0; i < strlen(letters); i++)  {
		for (int j=0; j < combination[i]; j++) {
			result[current] = letters[i];
			current++;
		}
	}
	result[current] = '\0';
	return result;
}

int decrement(int *combination, int length, int *resetter) {
	int i;
	int ret=1;
	for (i=length-1; i >= 0; i--) {
		if (combination[i] == 0) {
			combination[i] = resetter[i];
		} else {
			combination[i] -= 1;
			ret = 0;
			i = -1;
		}
	}
	return ret;
}

void scrabbler(treapset *word_set, char *letters) {
	int length;
	int *combination;
	int *resetter;
	int stop=0;

	combination = create_combo_array(letters, &length);
	resetter = (int *) emalloc(sizeof(int) * length);

	/* copying array combinations max values */
	for (int i=0; i < length; i++) {
		resetter[i] = combination[i];
	}
	
	char *word;
	while (stop == 0) {
		word = word_subset(letters, combination);
		word_searcher(word_set, word);
		free(word);
		stop = decrement(combination, length, resetter);
	}
	sort_words(output);
	destroy_treap(output, 0);
	output = NULL;
	free(combination);
	free(resetter);

}

char *sanitizer(char *word) {
	int length = strlen(word);
	int current = 0;
	char *result = (char *) emalloc(length);

	for (int i=0; i < length - 1; i++) {
		if ((word[i] <= 122) && (word[i] >= 97)) {
			result[current] = word[i];
			current++;
		} else if ((word[i] <= 90) && (word[i] >= 65)) {
			result[current] = word[i];
			current++;
		}
	}
	result[current] = '\0';
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
		char *word = sanitizer(input_buffer);
		scrabbler(word_set, word);
		free(word);
		printf("\n");
	}

	printf("\nFreeing memory...\n");
	destroy_treap(word_set, 1);
	printf("Done!\n");
	return 0;
}
