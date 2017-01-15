#include <list>
#include <array>
#include "combo.hpp"

//for upper-case letters
bool is_upper(int c) {
	return c >= 65 && c <= 90;
}

//for lower-case letters
bool is_lower(int c) {
	return c >= 97 && c <= 122;
}

bool is_letter(int c) {
	return is_lower(c) || is_upper(c);
}


//converts the ascii value into a number between 0 and 25
int normalize_letter(int c) {
	if (is_lower(c))
		return c - 97;
	else
		return c - 65;
}

Combo::Combo(std::string input) {
	int val;
	int normal;
	int bucket[26] = { 0 };
	int bucket_val;
	std::array<int, 2> element;
	std::string result;
	std::string::iterator it;

	//counting the number that each letter occurs and creates a new
	//string that has duplicate letters filtered out
	for (it = input.begin(); it < input.end(); ++it) {
		val = (int) *it;
		if (is_letter(val)) {
			normal = normalize_letter(val);
			if (bucket[normal] == 0)
				result += *it;
			bucket[normal] += 1;
		}
	}

	//putting the number of occurrences of each letter into an array
	//of two and pushing it into the linked list
	for (it = result.begin(); it != result.end(); ++it) {
		val = (int) *it;
		bucket_val = bucket[normalize_letter(val)];
		//element[0] will decrease in the same manner as a binary
		//counter
		element[0] = bucket_val;
		//element[1] will be constant so element[0] will be able to
		//reset to its original value
		element[1] = bucket_val;
		combo.push_back(element);
	}

	str = result;
}

//returns the current subset of letters
//example: if str is "cabb" and combo is "[1,1]->[0,1]->[2,2]"
//thent he resulting string will be "cbb"
std::string Combo::current() {
	int i;
	int j = 0;
	std::string result;
	std::list<std::array<int,2>>::iterator it;

	for (it=combo.begin(); it != combo.end(); ++it) {
		for (i=0; i < it->front(); i++) 
			result += str[j];
		j += 1;
	}

	return result;
}

//decrements the the combination array (combo) in the same manner as a
//binary counter (only modifies the first element)
//example: if combo is "[1,1]->[1,1]->[0,2]"
//after running the function combo will be "[1,1]->[0,1]->[2,2]"
//eventually, this function will reset combo to its initial value and
//will return false, indicating that the final subset has already
//been passed
bool Combo::decrement() {
	std::list<std::array<int, 2>>::reverse_iterator it;

	for (it=combo.rbegin(); it != combo.rend(); ++it) {
		if (it->front() == 0) {
			it->front() = it->back();
		} else {
			it->front() -= 1;
			return true;
		}
	}	

	return false;
}


