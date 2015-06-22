#ifndef H_AUTOMATA
#define H_AUTOMATA

#include <iostream>
#include <set> 
#include <string>
#include <unordered_map>
#include <vector>

class State;

class Automata {
	private:
		std::unordered_map< State*,  std::unordered_map<char , std::set<State*> > > m_transitions;
		std::vector<State*> m_states;
		State* m_initial;
		State* m_currentState;

		std::unordered_map<char, std::set<State*>> merge_transitions(std::set<State*> states);
		bool is_reachable(State* state);
		bool contains_epsilon();
		std::set<State*> parents_with_epsilon(State* state);
		State* find_son_with_epsilon();
		void merge_transitions(State* source, State* target);
		void erase_state(State* state);

	public:
		Automata();
		~Automata();
		Automata(std::unordered_map< State*,  std::unordered_map<char , std::set<State*> > > transitions);
		
		virtual void serialize(std::string& path);
		virtual void parse_file(std::string& path);
		virtual void parseWord(std::string& word) const;
		virtual bool isAcceptedWord(std::string& word);
		virtual void removeEpsilonTransitions();
		virtual void determinise();
		virtual void minimise();
		virtual void transpose();
		virtual bool isDeterminist() const;
		virtual bool containsEpsilonTransitions() const;
		virtual State* getInitial() const;
		virtual void setInitial(State* state);
		virtual State* getCurrentState() const;
		virtual void setCurrentState(int index);
		virtual std::set<State*> getTransition(State* state, char transition);
		virtual std::unordered_map<char , std::set<State*> > getStateTransitions(State* state);
		virtual State* getState(int index) const;
		virtual void addTransition(State* state, char transition, State* next);
		virtual void addStateTransitions(State* state, std::unordered_map<char , std::set<State*> > transitions);
		virtual void addState(State* state);
		virtual int nbState() const;
		virtual State* getStateByName(std::string name) const;
		virtual	std::unordered_map< State*,  std::unordered_map<char , std::set<State*> > > getTransitions() const;
		virtual void copyTransitions(Automata* newAutomata);
		virtual void copyStateTransitions(State* state, Automata* newAutomata);
		virtual void setStates(std::vector<State*> states);
		virtual void setCurrentState(State* state);

		friend std::ostream& operator<<(std::ostream& os, const Automata& automata);
};

#endif
