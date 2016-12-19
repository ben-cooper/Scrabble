#include "treapset.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*list of primes for the hashing function*/
const int primes[26] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43,
			47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101};

treapset *output = NULL;

struct combo {
	int *combo;
	int *reset;
	char *str;
	int length;
};

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

struct combo *create_combo_array(char *letters) {
	int bucket[26] = { 0 };
	int *combo;
	int *reset;
	struct combo *result = (struct combo *) emalloc(sizeof(struct combo));
	unsigned int next = 0;
	unsigned int index;
	char *str = (char *) emalloc(strlen(letters) + 1);
	strcpy(str, letters);

	fill_buckets(str, bucket);

	/* removing duplicate letters from input */
	for (index=0; index < 26; index++) {
		if (bucket[index] != 0) {
			str[next] = index + 97;
			next += 1;
		}
	}
	str[next] = '\0';


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


unsigned long power(int x, int y) {
	unsigned long result = (unsigned long) x;
	int i;

	if (y == 0) {
		return 1;
	}
	for (i=1; i < y; i++) {
		result *= x;
	}
	return result;
}

unsigned long word_hasher(char *str) {
	/*creating buckets to bucket sort (array of ones)*/
	int bucket[26] = { 0 };
	unsigned long result = 1;
	unsigned int index;

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

	/* if empty file */
	if (word_set == NULL) {
		fprintf(stderr, "Initialization failed! Empty File.\n");
		exit(1);
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

void scrabbler(treapset *word_set, char *letters) {
	struct combo *combination;
	int stop=0;
	char *word;

	combination = create_combo_array(letters);

	while (stop == 0) {
		word = word_subset(combination);
		word_searcher(word_set, word);
		free(word);
		stop = decrement(combination);
	}

	free(combination->combo);
	free(combination->reset);
	free(combination->str);
	free(combination);
	sort_words(output);
	destroy_treap(output, 0);
	output = NULL;

}

char *sanitizer(char *word) {
	int length = strlen(word);
	int current = 0;
	int i;
	char *result = (char *) emalloc(length);

	for (i=0; i < length - 1; i++) {
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
	destroy_treap(word_set, 1);
	printf("Done!\n");
	return 0;
}
