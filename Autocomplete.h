#ifndef AUTO_COMPLETE_HDR
#define AUTO_COMPLETE_HDR

#include <fstream>

#include "Automata.h"

class Autocomplete
{
public:
	Autocomplete(const std::string&, const size_t&);
	~Autocomplete();

public:
	void insertWord(const std::string&);
	void suggest(const std::string&);
	void changeSuggestionNumber(const size_t&);
	const size_t getSuggestionNumber() const { return suggestions_number; }

private:
	Automata au;
	size_t suggestions_number;

private:
	void editWord(std::string&);
};

#endif // !AUTO_COMPLETE_HDR
