#ifndef MENU_HDR
#define MENU_HDR

#include "Autocomplete.h"

class Menu
{
public:
	Menu();
	~Menu();

public:
	void menu();

private:
	size_t getIndex(const std::string&, const size_t&, const char&);
	std::string createString(const std::string&, const size_t&, size_t&, const char&);
	void help();
	size_t my_atoi(const std::string&);
	void createPathAndNumber(const std::string&, std::string&, size_t&);
};

#endif // !MENU_HDR
