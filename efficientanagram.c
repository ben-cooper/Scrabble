#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *word_sort(char *str) {
	//creating buckets to bucket sort
	int bucket[26] = { 0 }; //array of zereos
	
	int index;
	for (index=0; index < strlen(str); index++) {
		if ((str[index] >= 97) && (str[index] <= 122))
			bucket[str[index] - 97] += 1;

		if ((str[index] >= 65) && (str[index] <= 90))
			bucket[str[index] - 65] += 1;
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

int word_hash(char *str) {
	int index;
	int count = 0;
	for (index=0; index < strlen(str); index++) {
		count += (int) str[index];
	}
	return count;
}


void swap(char *x, char *y) {
	char temp;
	temp = *x;
	*x = *y;
	*y = temp;
}

void word_search(char *str) {
	//making buffer
	char word[100];
	char word_copy[100];

	//opening word file
	FILE *list;
	list = fopen("/usr/share/dict/words", "r");

	//printf("%s\n", str);

	while ((fgets(word, 100, list) != NULL)) {
		word[strlen(word) - 1] = '\0';
		strcpy(word_copy, word);
		if (strcmp(word_sort(word), word_sort(str)) == 0)
			printf("%s\n", word_copy);
		
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

	
	//permute(argv[1], 0, strlen(argv[1]));
	//printf("%d\n", word_hash(argv[1]));
	word_search(argv[1]);

	return 0;
}
