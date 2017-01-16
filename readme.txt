Scrabble Word Finder
(Written in C++ for Unix-like systems.)
(Will work on Windows as well.)

This program finds words you could make out of a subset of given letters, for English words.
To compile this program.  You must download:
-word.cpp
-combo.cpp
-combo.hpp
-Makefile

And then run the Makefile with the command 'make' if your are using Unix.

If you are on Windows, you will need to use Visual Studio or some other C++
compiler for Windows.

To run this program after compiling it, invoke "scrabble" from the terminal.
If you are on a Unix-like system, calling it without any parameters will
cause it to try to use the default word list in Unix.

Otherwise, you can pass the path to a word list in the same format
(one word on each line in plain text) as an argument for the program
to use instead.

If you don't have a word list handy, you can download twl.txt from the repo.
(This is the official word list for scrabble in North America)

Examples:
$ ./scrabble twl.txt
Initializing word list...
Done!
178691 words loaded.

Letters: cat
--------------------
at
ta
act
cat

Press Ctrl-D to exit the program on Unix.
Press Ctrl-Z and Enter to exit the program on Windows.

Enjoy!
