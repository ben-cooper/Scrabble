#include <list>
#include <array>
#include "combo.h"

bool is_upper(int c) {
	return c >= 65 && c <= 90;
}

bool is_lower(int c) {
	return c >= 97 && c <= 122;
}

bool is_letter(int c) {
	return is_lower(c) || is_upper(c);
}

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

	for (it = input.begin(); it < input.end(); ++it) {
		val = (int) *it;
		if (is_letter(val)) {
			normal = normalize_letter(val);
			if (bucket[normal] == 0)
				result += *it;
			bucket[normal] += 1;
		}
	}

	for (it = result.begin(); it != result.end(); ++it) {
		val = (int) *it;
		bucket_val = bucket[normalize_letter(val)];
		element[0] = bucket_val;
		element[1] = bucket_val;
		combo.push_back(element);
	}

	str = result;
}

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


