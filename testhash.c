#include <stdio.h>
#include <string.h>	
#include <stdlib.h>

unsigned long hashstring(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++) != 0)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

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
	if ((result = (unsigned char *) malloc(sizeof(char) * length)) == NULL) {
		fprintf(stderr, "Out of memory!\n");
		exit(1);
	}
	for (int i=0; i < 26; i++) {
		for (int j=0; j < bucket[i]; j++) {
			result[index] = (unsigned char) (i+97);
			index++;
		}
	}

	return result;

}


int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s word\n", argv[0]);
		return 1;
	}
	unsigned char *result = word_sorter((unsigned char *) argv[1]);
	printf("%s\n", result);
	printf("%lu\n", hashstring(result));
	result = word_sorter((unsigned char *) argv[1]);
	printf("%s\n", result);
	printf("%lu\n", hashstring(result));
	return 0;	
}
