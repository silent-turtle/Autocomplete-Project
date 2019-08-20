#include "Autocomplete.h"



Autocomplete::Autocomplete(const std::string & path, const size_t & suggestions = 10) : suggestions_number(suggestions)
{
	std::ifstream ifs(path);
	if (!ifs.is_open())
	{
		throw std::logic_error("File could not be opened");
	}

	std::vector<std::string> words;

	while (!ifs.eof())
	{
		std::string buffer;
		getline(ifs, buffer);
		editWord(buffer);
		if (buffer != "")
		{
			words.push_back(buffer);
		}
	}

	ifs.close();

	au.insertSorted(words);
}

Autocomplete::~Autocomplete()
{
}

void Autocomplete::insertWord(const std::string& word)
{
	au.insertUnsorted({ word });
}

void Autocomplete::suggest(const std::string& prefix)
{
	std::vector<std::string> suggestions = au.getSuggestions(prefix, suggestions_number);

	if (suggestions.empty())
	{
		std::cout << "there are no matches";
	}
	else
	{
		std::cout << "Suggestions: " << std::endl;

		for (size_t i = 0; i < suggestions.size(); ++i)
			std::cout << suggestions[i] << std::endl;
	}
}

void Autocomplete::changeSuggestionNumber(const size_t& num)
{
	suggestions_number = num;
}

void Autocomplete::editWord(std::string& w)
{
	std::string temp;
	size_t i = 0;

	while (w[i] != '\0')
	{
		while (w[i] == ' ' && w[i] != '\0')
		{
			++i;
		}

		while (w[i] != ' ' && w[i] != '\0')
		{
			temp += w[i];
			++i;
		}

		if (w[i] == ' ')
		{
			temp += w[i];
			++i;
		}
	}

	w = temp;
}
