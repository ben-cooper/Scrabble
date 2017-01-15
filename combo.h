#ifndef Combo_H
#define Combo_H


//these functions are used to fill the letter buckets
//that are used for generating subsets and hashes
extern bool is_upper(int);
extern bool is_lower(int);
extern bool is_letter(int);
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
