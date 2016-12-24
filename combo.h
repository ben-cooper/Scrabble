#ifndef Combo_H
#define Combo_H

extern bool is_upper(int);
extern bool is_lower(int);
extern bool is_letter(int);
extern int normalize_letter(int);

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
