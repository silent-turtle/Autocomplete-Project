#include "Menu.h"



Menu::Menu()
{
}


Menu::~Menu()
{
}

void Menu::menu()
{
	std::cout << "Please, enter a filepath and the number of suggestions you'd like to get!\n";
	
	std::string buffer;
	std::getline(std::cin, buffer);

	std::string path;
	size_t suggestion_number;

	createPathAndNumber(buffer, path, suggestion_number);

	Autocomplete ac(path, suggestion_number);

	help();

	std::string command;
	do
	{
		try
		{
			std::cout << "\n>";
			std::getline(std::cin, buffer);
			size_t begin = 0, end = 0;

			command = createString(buffer, begin, end, ' ');

			if (buffer[end] != '\0')
			{
				while (buffer[end] == ' ')
					++end;

				begin = end;

				if (buffer[end] == '\"')
				{
					begin = end + 1;
				}
			}

			if (command == "suggest")
			{
				std::string sugg = createString(buffer, begin, end, '\"');

				if (sugg.size() == 0)
				{
					std::cout << "There is no input";
					continue;
				}

				ac.suggest(sugg);

				continue;
			}

			if (command == "getSuggestionNumber")
			{
				std::cout << ac.getSuggestionNumber();
				continue;
			}

			if (command == "changeSuggestionNumber")
			{
				std::string number = createString(buffer, begin, end, '\0');
				size_t new_num = my_atoi(number);
				ac.changeSuggestionNumber(new_num);

				continue;
			}

			if (command == "insert")
			{
				std::string word = createString(buffer, begin, end, '\"');

				ac.insertWord(word);
				continue;
			}

			if (command == "help")
			{
				help();
				continue;
			}

			if (command != "exit")
			{
				std::cout << "Unknown command!\n\n";

				std::cin.clear();
			}

		}
		catch (std::exception& e)
		{
			std::cout << e.what();

			if (!strcmp(e.what(), "...and the music stops!"))
				break;
		}
	} while (command != "exit");

}

size_t Menu::getIndex(const std::string& str, const size_t& begin, const char& c)
{
	size_t res = begin;

	while (str[res] != c && str[res] != '\0')
	{
		++res;
	}

	return res;
}

std::string Menu::createString(const std::string& str, const size_t& begin, size_t& end, const char& c)
{
	std::string temp;

	end = getIndex(str, begin, c);

	temp = str.substr(begin, end - begin);

	if (str[end] != '\0')
		++end;

	return temp;
}

void Menu::help()
{
	std::cout << "What would you like to do?\n"
		<< "1> insert <\"word\">;\n"
		<< "2> suggest <\"prefix\">;\n"
		<< "3> getSuggestionNumber;\n"
		<< "4> changeSuggestionNumber <number>;\n"
		<< "5> help;\n"
		<< "6> exit.\n";
}

size_t Menu::my_atoi(const std::string& str)
{
	size_t res = 0;

	for (size_t i = 0; i < str.size(); ++i)
	{
		res *= 10;
		res += str[i] - '0';
	}

	return res;
}

void Menu::createPathAndNumber(const std::string& buff, std::string& path, size_t& num)
{
	size_t end = buff.size();

	while (buff[end] == ' ')
	{
		--end;
	}
	size_t begin = end;

	while (buff[begin] != ' ')
	{
		--begin;
	}
	num = my_atoi(buff.substr(begin + 1, end));

	end = begin;

	while (buff[end] == ' ')
	{
		--end;
	}
	begin = 0;

	path = buff.substr(begin, end + 1);
}

