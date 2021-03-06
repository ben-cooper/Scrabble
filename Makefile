fast: hashtable.c hashtable.h scrabble.c
	gcc -Wall -Wextra -Wpedantic --std=c11 -O3 scrabble.c hashtable.c -o scrabble

debug: hashtable.c hashtable.h scrabble.c
	gcc -Wall -Wextra -Wpedantic --std=c11 -g scrabble.c hashtable.c -o scrabble

small: hashtable.c hashtable.h scrabble.c
	gcc -Wall -Wextra -Wpedantic --std=c11 -Os scrabble.c hashtable.c -o scrabble

32: hashtable.c hashtable.h scrabble.c
	gcc -Wall -Wextra -Wpedantic --std=c11 -O3 -m32  scrabble.c hashtable.c -o scrabble
