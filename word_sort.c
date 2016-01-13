#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *word_sort(char *str) {
	//creating buckets to bucket sort
	int bucket[26] = { 0 }; //array of zereos
	
	int index;
	for (index=0; index < strlen(str); index++) {
		if ((str[index] >= 97) && (str[index] <= 122))
			bucket[str[index] - 97] += 1;
	}
	//creating string buffer
	char buffer[100] = {'\0'};
	int counter;
	char sorted[2]= { '\0' };


	for (index=0; index < 26; index++) {
		sorted[0]  = (char) (index+97);
		for (counter=0; counter < bucket[index]; counter++) {
			strcat(buffer, sorted);
		}
	}
	strcpy(str, buffer);
	return str;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s word", argv[0]);
		exit(1);
	}

	printf("%s\n", word_sort(argv[1]));
	return 0;
}

