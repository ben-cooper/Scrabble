fast: treapset.c treapset.h scrabble.c
	gcc -Wall -Wextra -Wpedantic --std=c11 -Ofast scrabble.c treapset.c -o scrabble

debug: treapset.c treapset.h scrabble.c
	gcc -Wall -Wextra -Wpedantic --std=c11 -g scrabble.c treapset.c -o scrabble

