#include "Automata.h"


Automata::Automata() : start_state(nullptr)
{
}


Automata::~Automata()
{
}

void Automata::insertSorted(std::vector<std::string>& words)
{
	std::sort(words.begin(), words.end());

	start_state = new State({}, 0, false);

	for (size_t i = 0; i < words.size(); ++i)
	{
		std::string word = words[i];
		std::string prefix = common_prefix(words[i]);
		State* last_state = transition(start_state, prefix);
		std::string suffix = words[i].substr(prefix.size(), words[i].size());

		if (hasChildren(last_state))
		{
			replaceOrRegisterSorted(last_state);
		}

		add_suffix(last_state, suffix);
	}

	replaceOrRegisterSorted(start_state);
}

void Automata::insertUnsorted(const std::vector<std::string>& words)
{
	for (size_t i = 0; i < words.size(); ++i)
	{
		std::string prefix = common_prefix(words[i]);
		std::string suffix = words[i].substr(prefix.size(), words[i].size());
		if (suffix == "")
		{
			return;
		}

		State* first_state = firstConfluenceState(prefix);

		clone(prefix, first_state);

		State* last_state = transition(start_state, prefix);

		add_suffix(last_state, suffix);

		replaceOrRegisterUnsorted(start_state);
	}
}

std::vector<std::string> Automata::getSuggestions(const std::string& prefix, const size_t& num)
{
	State* res_state = transition(start_state, prefix);

	if (!res_state)
	{
		return std::vector<std::string>{};
	}

	std::vector<std::string> res = res_state->getRightLanguage();

	size_t min = std::min(num, res.size());

	for (size_t i = 0; i < min; ++i)
	{
		res[i] = prefix + res[i];
	}

	return res;
}

std::string Automata::common_prefix(const std::string& w)
{
	State* current = start_state;
	std::string prefix;
	for (size_t i = 0; i < w.size(); ++i)
	{
		std::unordered_map<std::pair<State*, char>, State*>::iterator it = transitions.find(std::make_pair(current, w[i]));
		if (it == transitions.end())
		{
			return prefix;
		}

		current->addToRightLanguage(w.substr(i, w.size()));
		prefix += w[i];
		current = transitions[std::make_pair(current, w[i])];
	}
}

State* Automata::transition(State* q, const std::string& w)
{
	State* current = q;
	for (size_t i = 0; i < w.size(); ++i)
	{
		if (transitions.find(std::make_pair(current, w[i])) == transitions.end())
		{
			return nullptr;
		}

		current = transitions[std::make_pair(current, w[i])];
	}

	return current;
}

bool Automata::hasChildren(State* q)
{
	for (std::unordered_map<std::pair<State*, char>, State*>::iterator it = transitions.begin(); it != transitions.end(); ++it)
	{
		if (it->first.first == q)
		{
			return true;
		}
	}

	return false;
}

State* Automata::last_child(State* q)
{
	return transitions[std::make_pair(q, q->getLastChildSymbol())];
}

void Automata::replaceOrRegisterSorted(State* q)
{
	State* child = last_child(q);
	if (child)
	{
		if (hasChildren(child))
		{
			replaceOrRegisterSorted(child);
		}

		if (reg.empty())
		{
			reg.push_back(child);
			return;
		}

		bool areEqual = false;

		for (size_t i = 0; i < reg.size(); ++i)
		{
			if (statesAreEqual(reg[i], child))
			{
				areEqual = true;
				transitions[std::make_pair(q, q->getLastChildSymbol())] = reg[i];

				q->removeChild(q->getLastChildSymbol(), child);
				q->addChild(q->getLastChildSymbol(), reg[i]);

				delete child;
			}
		}

		if (!areEqual)
		{
			reg.push_back(child);
		}
	}
}

void Automata::replaceOrRegisterUnsorted(State* q)
{
	std::vector <std::pair<char, State*>> children = getChildren(q);
	for (size_t i = 0; i < children.size(); ++i)
	{
		State* child = children[i].second;

		if (hasChildren(child))
		{
			replaceOrRegisterUnsorted(child);
		}

		if (reg.empty())
		{
			reg.push_back(child);
			return;
		}

		bool areEqual = false;

		for (size_t i = 0; i < reg.size(); ++i)
		{
			if (reg[i] == child)
			{
				areEqual = true;
				break;
			}
			if (statesAreEqual(reg[i], child))
			{
				areEqual = true;
				transitions[std::make_pair(q, q->getLastChildSymbol())] = reg[i];
				delete child;
				break;
			}
		}

		if (!areEqual)
		{
			reg.push_back(child);
		}
	}
}

bool Automata::statesAreEqual(State* q, State* p)
{
	if (q->isFinal() != p->isFinal())
	{
		return false;
	}

	std::vector<std::pair<char, State*>> q_children = getChildren(q);
	std::vector<std::pair<char, State*>> p_children = getChildren(p);


	if (q_children.size() != p_children.size())
	{
		return false;
	}

	std::sort(q_children.begin(), q_children.end(), customLess);
	std::sort(p_children.begin(), p_children.end(), customLess);

	for (size_t i = 0; i < q_children.size(); ++i)
	{
		if (q_children[i].second != p_children[i].second || q_children[i].first != p_children[i].first)
		{
			return false;
		}
	}

	return true;
}

std::vector<std::pair<char, State*>> Automata::getChildren(State* q)
{
	return q->getChildren();
}

void Automata::add_suffix(State* q, const std::string& suff)
{
	State* current = q;
	current->addToRightLanguage(suff);

	for (size_t i = 0; i < suff.size() - 1; ++i)
	{
		transitions[std::make_pair(current, suff[i])] = new State({ suff.substr(i + 1, suff.size()) }, 0, false);

		if (current->getLastChildSymbol() < suff[i])
		{
			current->setLastChildSymbol(suff[i]);
		}

		current->addChild(suff[i], transitions[std::make_pair(current, suff[i])]);
		current = transitions[std::make_pair(current, suff[i])];
	}

	transitions[std::make_pair(current, suff[suff.size() - 1])] = new State({ "" }, 0, true);

	if (current->getLastChildSymbol() < suff[suff.size() - 1])
	{
		current->setLastChildSymbol(suff[suff.size() - 1]);
	}

	current->addChild('\0', transitions[std::make_pair(current, suff[suff.size() - 1])]);
}

State* Automata::firstConfluenceState(const std::string& w)
{
	State* current = start_state;

	for (size_t i = 0; i < w.size(); ++i)
	{
		State* child = transitions[std::make_pair(current, w[i])];

		for (std::unordered_map<std::pair<State*, char>, State*>::iterator it = transitions.begin(); it != transitions.end(); ++it)
		{
			if ((it->second == child) && (it->first != std::make_pair(current, w[i])))
			{
				return child;
			}
		}
		current = child;
	}

	return nullptr;
}

void Automata::clone(const std::string& w, State* start)
{
	State* current = start_state;
	State* child = start_state;
	char transition_symbol = 0;
	size_t cnt = 0;
	while (child != start)
	{
		current = child;
		child = transitions[std::make_pair(current, w[cnt])];
		transition_symbol = w[cnt];
		++cnt;
	}

	State* res;

	clone_rec(child, res);
	transitions[std::make_pair(current, transition_symbol)] = res;
}

void Automata::clone_rec(State* curr, State*& new_curr)
{
	if (hasChildren(curr))
	{
		State* temp = new State(*curr);
		std::vector<std::pair<char, State*>> children = getChildren(curr);
		for (size_t i = 0; i < children.size(); ++i)
		{
			State* res = nullptr;

			clone_rec(children[i].second, res);

			if (children[i].second->isFinal())
			{
				res = children[i].second;
			}

			transitions[std::make_pair(temp, children[i].first)] = res;
		}
		new_curr = temp;
	}
}
