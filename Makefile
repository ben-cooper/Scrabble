fast: treapset.c treapset.h fullanagram.c
	gcc -Wall -Wextra -Wpedantic --std=c11 -Ofast fullanagram.c treapset.c -o fullanagram

debug: treapset.c treapset.h fullanagram.c
	gcc -Wall -Wextra -Wpedantic --std=c11 -Ofast -g fullanagram.c treapset.c -o debuganagram

