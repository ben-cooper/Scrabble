#ifndef Combo_H
#define Combo_H
#include <list>
#include <array>
#include <string>

//these functions are used to fill the letter buckets
//that are used for generating subsets and hashes
extern int normalize_letter(int);

//this class takes a string in its constructor and will create a new subset
//of letters every time decrement is called
//current will return the current subset as a string
class Combo {
	private:
	 std::list<std::array<int,2>> combo;
	 std::string str;
	public:
	 explicit Combo(std::string);
	 std::string current();
	 bool decrement();
};

#endif
