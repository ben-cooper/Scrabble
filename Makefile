fast: word.cpp combo.cpp combo.hpp
	g++ -Wall -Wextra -Wpedantic --std=c++11 -Ofast word.cpp combo.cpp -o scrabble

debug: word.cpp combo.cpp combo.hpp
	g++ -Wall -Wextra -Wpedantic --std=c++11 -g word.cpp combo.cpp -o scrabble

small: word.cpp combo.cpp combo.hpp
	g++ -Wall -Wextra -Wpedantic --std=c++11 -Os word.cpp combo.cpp -o scrabble

32: word.cpp combo.cpp combo.hpp
	g++ -Wall -Wextra -Wpedantic --std=c++11 -Ofast -m32  word.cpp combo.cpp -o scrabble
