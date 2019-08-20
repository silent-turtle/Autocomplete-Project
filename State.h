#ifndef STATE_HDR
#define STATE_HDR

#include <vector>
#include <string>

class State
{
public:
	State(const std::vector<std::string>&, const char&, const bool&);
	State(const State&);
	~State();

public:
	void setLastChildSymbol(char);
	const char getLastChildSymbol() const { return last_child_symbol; }
	
	bool isFinal() const { return final; }
	
	void addToRightLanguage(const std::string& w);
	const std::vector<std::string>& getRightLanguage() const { return right_language; }
	
	void addChild(const char&, State*);
	void removeChild(const char&, State*);
	const std::vector<std::pair<char, State*>>& getChildren() const { return children; }

	const size_t getStateNumber() const { return state_number; }

private:
	static size_t states_count;
	size_t state_number;
	std::vector<std::string> right_language;
	std::vector<std::pair<char, State*>> children;
	char last_child_symbol;
	bool final;
};

#endif // !STATE_HDR
