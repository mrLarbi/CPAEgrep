#ifndef H_Automata
#define H_Automata

#include <string>
#include <unordered_map>
#include <vector>
#include <set>

class State;

class Automata;

class IAutomata {

	public:
		virtual void serialize(std::string& path) = 0;
		virtual void parse_file(std::string& path) = 0;
		virtual void parseWord(std::string& word) const = 0;
		virtual bool isAcceptedWord(std::string& word) = 0;
		virtual void removeEpsilonTransitions() = 0;
		virtual void determinise() = 0;
		virtual void minimise() = 0;
		virtual void transpose() = 0;
		virtual bool isDeterminist() const = 0;
		virtual bool containsEpsilonTransitions() const = 0;
		virtual State* getInitial() const = 0;
		virtual void setInitial(State* state) = 0;
		virtual State* getCurrentState() const = 0;
		virtual void setCurrentState(int index) = 0;
		virtual void setCurrentState(State* state) = 0;
		virtual std::set<State*> getTransition(State* state, char transition) = 0;
		virtual std::unordered_map<char , std::set<State*> > getStateTransitions(State* state) = 0;
		virtual State* getState(int index) const = 0;
		virtual void addTransition(State* state, char transition, State* next) = 0;
		virtual void addStateTransitions(State* state, std::unordered_map<char , std::set<State*> > transitions) = 0;
		virtual void addState(State* state) = 0;
		virtual int nbState() const = 0;
		virtual State* getStateByName(std::string name) const = 0;
		virtual void copyTransitions(Automata* newAutomata) = 0;
		virtual void copyStateTransitions(State* state, Automata* newAutomata) = 0;
		virtual void setStates(std::vector<State*> states) = 0;
};

#endif
