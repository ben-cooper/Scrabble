#include <iostream>
#include <fstream>
#include <unordered_map>
#include <list>
#include <string>
#include <array>
#include "combo.h"

const int primes[26] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43,
			47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101};

bool is_anagram(std::string &str1, std::string &str2) {
		int val;
		int i;
		int buckets[26] = { 0 };
		std::string::iterator it;

		for (it=str1.begin(); it != str1.end(); ++it) {
		val = (int) *it;
			if (is_letter(val))
				buckets[normalize_letter(val)] += 1;
		}

		for (it=str2.begin(); it != str2.end(); ++it) {
		val = (int) *it;
			if (is_letter(val))
				buckets[normalize_letter(val)] -= 1;
		}

		for (i=0; i < 26; i++) 
			if (buckets[i] != 0)
				return false;
		return true;
}

unsigned long word_hasher(std::string &str) {
	unsigned long result = 1;
	std::string::iterator it;
	int val;

	for (it = str.begin(); it < str.end(); ++it) {
	val = (int) *it;
		if (is_letter(val))
			result *= primes[normalize_letter(val)];
	}

	return result;
}

bool compare_length(std::string &first, std::string &second) {
	return first.size() < second.size();
}

void word_searcher(std::string &letters, std::unordered_map<unsigned long int, 
		std::list<std::string>> &word_set) {

	Combo combine(letters);
	std::string line;
	std::list<std::string> result;
	std::list<std::string>::iterator it;
	unsigned long hash;

	do {
		line = combine.current();
		hash = word_hasher(line);
		if (word_set.find(hash) != word_set.end()) {
			for (it=word_set[hash].begin();
			it != word_set[hash].end(); ++it) {
				if (is_anagram(line, *it))
					result.push_front(*it);
			}
		}
	} while (combine.decrement());

	result.sort(compare_length);
	for (it=result.begin(); it != result.end(); ++it) {
		std::cout << *it << std::endl;
	}
	std::cout << std::endl;
}

int main(int argc, char **argv) {
	unsigned long word_count = 0;
	std::ifstream dict;
	std::string letters;
	std::string line;
	std::unordered_map<unsigned long int, std::list<std::string>> word_set;

	if (argc == 1) {
		dict.open("/usr/share/dict/words");
		if (!dict.is_open()) {
			std::cerr << "Unix word list not found.  \
				Please specify your own." << std::endl;
			exit(1);
		}
	} else if (argc == 2) {
		dict.open(argv[1]);
		if (!dict.is_open()) {
			std::cerr << "Could not open: " << argv[1] << std::endl;
			exit(1);
		}
	} else {
		std::cerr << "usage: " << argv[0] << " [path_to_word_list]"
			<< std::endl;
		exit(1);
	}

	std::cout << "Initializing word list..." << std::endl;
	while (getline(dict, line)) {
		word_set[word_hasher(line)].push_front(line);
		word_count += 1;
	}

	std::cout << "Done!" << std::endl;
	std::cout << word_count << " words loaded." << std::endl << std::endl
		<< "Letters: ";
	while (std::cin >> letters) {
		std::cout << "--------------------" << std::endl;
		word_searcher(letters, word_set);
		std::cout << std::endl <<  "Letters: ";
	}
	dict.close();
	std::cout << std::endl << "Goodbye." << std::endl;
}
