#include "State.h"

size_t State::states_count;

State::State(const std::vector<std::string>& r_l, const char& last_child = 0, const bool& isFinal = false) : 
	right_language(r_l), last_child_symbol(last_child), final(isFinal), state_number(states_count++)
{
	;
}

State::State(const State& other) : right_language(other.right_language), last_child_symbol(other.last_child_symbol), 
final(other.final), state_number(states_count++)
{
}


State::~State()
{
}

void State::setLastChildSymbol(char c)
{
	last_child_symbol = c;
}

void State::addToRightLanguage(const std::string & w)
{
	right_language.push_back(w);
}

void State::addChild(const char& c, State* p)
{
	children.push_back(std::make_pair(c, p));
}

void State::removeChild(const char& c, State* q)
{
	for (std::vector<std::pair<char, State*>>::iterator elem = children.begin(); elem != children.end(); ++elem)
	{
		if (elem->first == c && elem->second == q)
		{
			elem = children.erase(elem);
		}
	}
}
