#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void swap(char *x, char *y) {
	char temp;
	temp = *x;
	*x = *y;
	*y = temp;
}

void word_search(char *str) {
	//making buffer
	char word[100];

	//opening word file
	FILE *list;
	list = fopen("/usr/share/dict/words", "r");

	//printf("%s\n", str);

	while ((fgets(word, 100, list) != NULL)) {
		word[strlen(word) - 1] = '\0';
		if (strcmp(word, str) == 0)
			printf("%s\n", str);
		
		//printf("%s\n", word);
	}	

	fclose(list);
}

void permute(char *str, int start, int end) {

	int index;
	if (start == end) {
		word_search(str);
	} else {
		for (index = start; index < end; index++) {
			swap((str+start), (str+index));
			permute(str, start+1, end);
			swap((str+start), (str+index));
		}
	}
}

int main(int argc, char** argv) {
	//TESTS
	/*
	char str[] = "ben";
	permute(str, 0, 3);
	*/
	if (argc != 2) {
		fprintf(stderr, "usage: %s word\n", argv[0]);
		exit(1);
	}

	
	permute(argv[1], 0, strlen(argv[1]));

	return 0;
}
