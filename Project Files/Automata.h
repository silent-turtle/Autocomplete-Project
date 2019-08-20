#ifndef AUTOMATA_HDR
#define AUTOMATA_HDR

#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <set>

#include "State.h"

//using custom-made hash function for the transition function since a custom class is used
class Hasher
{
public:
	size_t operator()(const State* s) const
	{
		return std::hash<size_t>()(s->getStateNumber());
	}

	size_t operator()(const std::pair<State*, char>& p) const
	{
		size_t h1 = operator()(p.first);
		size_t h2 = std::hash<char>()(p.second);

		return ((h1 + h2)*(h1 + h2 + 1) / 2 + h2);
	}
};

class Automata
{
public:
	Automata();
	~Automata();


public:
	//used when initializing an automata with words
	void insertSorted(std::vector<std::string>&);
	
	//used when inserting new words to the already constructed automata
	//the idea of both algorithms is taken from here: https://www.aclweb.org/anthology/J00-1002
	void insertUnsorted(const std::vector<std::string>&);
	
	std::vector<std::string> getSuggestions(const std::string&, const size_t&);

private:
	std::unordered_map<std::pair<State*, char>, State*, Hasher> transitions;
	State* start_state;
	std::vector<State*> reg;

private:
	//return the prefix of the string that's already in the automata
	std::string common_prefix(const std::string&);

	//an upgrade of the transition function working with strings not just 1 symbol
	State* transition(State* q, const std::string&);
	
	bool hasChildren(State*);
	//returns the child of a state with the lexicographically last existing transition 
	State* last_child(State*);

	//the function that minimizes the automata when the function insertWordsSorted is called
	void replaceOrRegisterSorted(State*);
	
	//the function that minimizes the automata when the function insertWordsUnsorted is called
	//both functions work on the principle of equality between states, meaning it 
	//replaces a state from an equivalent class if another one from the same class already exists
	void replaceOrRegisterUnsorted(State*);
	
	bool statesAreEqual(State*, State*);
	std::vector<std::pair<char, State*>> getChildren(State*);
	void add_suffix(State*, const std::string&);

	State* firstConfluenceState(const std::string&);
	//function that clones every state that's after the one in the argument list (including it)
	void clone(const std::string&, State*);
	
	//helper for the above function
	void clone_rec(State*, State*&);

	//an overloaded comparison function for sorting the vector of the children of a state
	struct
	{
		bool operator()(const std::pair<char, State*>& op1, const std::pair<char, State*>& op2) { return op1.first < op2.first; }
	}customLess;
};

#endif // !AUTOMATA_HDR
