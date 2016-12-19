fast: word.cpp
	g++ -Wall -Wextra -Wpedantic --std=c++11 -Ofast word.cpp -o scrabble

debug: word.cpp
	g++ -Wall -Wextra -Wpedantic --std=c++11 -g word.cpp -o scrabble

small: word.cpp
	g++ -Wall -Wextra -Wpedantic --std=c++11 -Os word.cpp -o scrabble

32: word.cpp
	g++ -Wall -Wextra -Wpedantic --std=c++11 -Ofast -m32  word.cpp -o scrabble
