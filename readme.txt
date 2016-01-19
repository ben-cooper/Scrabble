# Anagram
Anagram Finder Written in C for Unix-like systems
(Will work on Windows as well.)

This program finds anagrams for English words.

To compile this program.  You must download:
-treapset.c
-treapset.h
-fullanagram.c
-Makefile

And then run the Makefile with the command 'make'.

To run this program after compiling it, invoke fullanagram from the terminal.

If you are on a Unix-like system, calling it without any parameters will
cause it to try to use the default word list in Unix.

Otherwise, you can pass the path to a word list in the same format
(one word on each line in plain text) as an argument for the program
to use instead.

If you don't have a word list handy, you can download dict.txt from the repo.

Examples:
> ./fullanagram
Initializing word list...
Number of words: 128985
Height: 40
Done!

cat

act
tac
cat



> ./fullanagram /home/ben/wordlist.txt
...

Press Ctrl-D to exit the program.

Enjoy!
