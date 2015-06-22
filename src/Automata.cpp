#include <algorithm>
#include <fstream>
#include <sstream>

#include "Automata.h"
#include "AutomataPrinter.h"
#include "State.h"

Automata::Automata()
{
	m_currentState = 0;
	m_initial = 0;
}

Automata::~Automata()
{
	for ( int i = 0; i < m_states.size(); i++ )
	{
		delete m_states[i];
	}
}

Automata::Automata(std::unordered_map< State*,  std::unordered_map<char , std::set<State*> > > transitions)
{
	this->m_transitions = transitions;
}

void Automata::parseWord(std::string& word) const
{
	// TODO
}

void Automata::serialize(std::string& path)
{
	const void * address = static_cast<const void*>(this);
	std::stringstream ss;
	ss << address;  
	std::string name = ss.str();

	std::string file_name = "automata.dot";

	std::ofstream myfile;
	myfile.open (path);

	myfile << "digraph graphname {\n";

	myfile << "	" << "s_start" << "[label = start, shape = none];\n";

	std::string same_rank = "";

	for(auto state : this->m_states)
	{
		if(state->isFinal())
		{
			myfile << "	" << "s_" << state->getValue() << "	[color = Blue, shape = doublecircle];\n";
			same_rank += " s_" + state->getValue();
		}
	}

	myfile << "	{rank = same" << same_rank << "};\n";

	myfile << "\n";

	myfile << "	s_start	->	s_" << this->m_initial->getValue() << ";\n";

	for(auto state_map : this->m_transitions)
	{
		for(auto char_set : state_map.second)
		{
			for(auto state : char_set.second)
			{
				if(char_set.first == 0)
				{
					myfile << "	s_" << state_map.first->getValue() << "	->	s_" << state->getValue() << "	[label=\"&epsilon;\"];\n";
				}
				else {
					myfile << "	s_" << state_map.first->getValue() << "	->	s_" << state->getValue() << "	[label=\"" << char_set.first << "\"];\n";
				}
			}
		}
	}

	myfile << "\n}";

	myfile.close();
}

void Automata::parse_file(std::string& path)
{
	std::ifstream file(path);
	std::string line;
	
	while (std::getline(file, line))
	{
		if(isAcceptedWord(line))
		{
			std::cout << path << ": " << line << std::endl;
		}
	}

	return;
}

bool Automata::isAcceptedWord(std::string& word)
{
	int index = 0;
	std::string working_copy = word;

	while(working_copy != "")
	{
		if(this->m_currentState->isFinal())
		{

			this->m_currentState = this->m_initial;
			return true;
		}

		char current_caracter = working_copy[index];

		int contains_key = this->m_transitions[this->m_currentState].count(current_caracter);

		if(contains_key)
		{
			this->m_currentState = *(this->m_transitions[m_currentState][current_caracter].begin());
			index++;
		}
		else {
			this->m_currentState = this->m_initial;
			working_copy.erase(0, 1);
			index = 0;
			continue;
		}
	}

	if(this->m_currentState->isFinal())
	{

		this->m_currentState = this->m_initial;
		return true;
	}


	this->m_currentState = this->m_initial;
	return false;
}

void Automata::removeEpsilonTransitions()
{

	while(contains_epsilon())
	{
		State* k = find_son_with_epsilon();

		std::set<State*> q_set = parents_with_epsilon(k);

		for(auto q : q_set)
		{
			merge_transitions(q, k);
			this->m_transitions[q][0].erase(k);

			if(this->m_transitions[q][0].size() == 0)
				this->m_transitions[q].erase(0);
		}

		if(! is_reachable(k))
		{
			erase_state(k);
		}
	}
}

bool Automata::is_reachable(State* state)
{
	if(state == this->m_initial)
		return true;

	for(auto state_map : this->m_transitions)
	{
		if(state_map.first != state)
		{
			for(auto char_set : this->m_transitions[state_map.first])
			{
				if(char_set.second.find(state) != char_set.second.end()) {
					return true;
				}
			}
		}
	}

	return false;
}

bool Automata::contains_epsilon()
{
	for(auto state : this->m_states)
	{
		int contains_epsilon = this->m_transitions[state].count(0);

		if(contains_epsilon && this->m_transitions[state][0].size() > 0) {
			return true;
		}
	} 
	
	return false;
}

std::set<State*> Automata::parents_with_epsilon(State* state)
{
	std::set<State*> result;

	for(auto state_to_check : this->m_states)
	{
		int contains_epsilon = m_transitions[state_to_check].count(0);

		if(contains_epsilon)
		{
			std::set<State*> set_to_check = this->m_transitions[state_to_check][0];

			if(set_to_check.find(state) != set_to_check.end())
				result.insert(state_to_check);
		}
	}

	return result;
}

State* Automata::find_son_with_epsilon()
{
	for(auto state : this->m_states)
	{
		int contains_epsilon = m_transitions[state].count(0);

		if(contains_epsilon)
		{
			return *(this->m_transitions[state][0].begin());
		}
	}

	return NULL;
}

void Automata::merge_transitions(State* source, State* target)
{
	if(target->getType() == state_type::FINAL)
		source->setType(state_type::FINAL);

	for(auto char_set : this->m_transitions[target])
	{
		char key = char_set.first;

		if(! (this->m_transitions[source].count(key)))
		{
			m_transitions[source][key] = char_set.second;
		}
		else
		{
			std::set<State*> states_to_add = char_set.second;
			std::set<State*> states_to_enlarge = this->m_transitions[source][key];

			states_to_enlarge.insert(states_to_add.begin(), states_to_add.end());

			this->m_transitions[source][key] = states_to_enlarge;
		}
	}
}

void Automata::erase_state(State* state)
{
	this->m_states.erase(std::remove(this->m_states.begin(), this->m_states.end(), state), this->m_states.end());

	this->m_transitions.erase(state);

	for(auto state_map : this->m_transitions)
	{
		for(auto char_set : state_map.second)
		{
			char_set.second.erase(state);
		}
	}
}

void Automata::setStates(std::vector<State*> states)
{
	this->m_states = states;
}

void Automata::determinise()
{
	if( ! isDeterminist())
	{
		std::unordered_map<State*,  std::unordered_map<char, std::set<State*>>> transitions_prime;
		std::vector<State*> states_prime;
		std::unordered_map<State*,  std::unordered_map<char, std::set<State*>>> todo;
		std::set<State*> states_done;

		todo[this->m_initial] = this->m_transitions[this->m_initial];
		
		while( ! todo.empty())
		{
			State* current_state = todo.begin()->first;
			
			states_done.insert(current_state);

			if(states_prime.empty())
			{
				states_prime.push_back(current_state);
			}
			else
			{
				if(std::find(states_prime.begin(), states_prime.end(), current_state) == states_prime.end())
					states_prime.push_back(current_state);
			}
			
			for(std::unordered_map<char, std::set<State*>>::iterator it = todo[current_state].begin(); it != todo[current_state].end(); it++)
			{
				char current_letter = it->first;
				std::set<State*> current_nex_states = it->second;

				State* state_prime = new State(current_nex_states);

				bool already_done = false;

				for(auto it_state : states_done)
				{
					if((state_prime->getValue().compare(it_state->getValue())) == 0)
					{
						state_prime = it_state;
						already_done = true;
						break;
					}
				}

				if( ! already_done)
				{
					std::unordered_map<char, std::set<State*>> merged_trans = merge_transitions(current_nex_states);
					todo[state_prime] = merged_trans;
				}

				std::set<State*> set_prime;
				set_prime.insert(state_prime);
				transitions_prime[current_state][current_letter] = set_prime;
			}

			todo.erase(current_state);
		}

		this->m_transitions = transitions_prime;
		this->m_states = states_prime;
	}
}

std::unordered_map<char, std::set<State*>> Automata::merge_transitions(std::set<State*> states)
{
	std::unordered_map<char, std::set<State*>> result;

	for(auto state : states)
	{
		for(auto key_values : this->m_transitions[state])
		{
			int contains_key = result.count(key_values.first);

			if( ! contains_key)
			{
				result[key_values.first] = key_values.second;
			}
			else
			{
				std::set<State*> temp_set = result[key_values.first];
				temp_set.insert(key_values.second.begin(), key_values.second.end());
				result[key_values.first] = temp_set;
			}
		}
	}

	return result;
}

void Automata::minimise()
{
	transpose();
	determinise();
	transpose();
	determinise();
}

void Automata::transpose()
{
	std::unordered_map<State*,  std::unordered_map<char, std::set<State*>>> transitions_prime;
	State* temp_initial = this->m_initial;

	for(auto state_map : this->m_transitions)
	{
		for(auto char_set : state_map.second)
		{
			for(auto state : char_set.second)
			{
				int contains_state_key = transitions_prime.count(state);

				if( ! contains_state_key)
				{
					std::set<State*> temp_set;
					temp_set.insert(state_map.first);
					std::unordered_map<char, std::set<State*>> temp_map;
					temp_map[char_set.first] = temp_set;
					transitions_prime[state] = temp_map;
				}
				else
				{
					std::unordered_map<char, std::set<State*>> temp_map = transitions_prime[state];
					int contains_char_key = temp_map.count(char_set.first);

					if( ! contains_char_key)
					{
						std::set<State*> temp_set;
						temp_set.insert(state_map.first);
						temp_map[char_set.first] = temp_set;
						transitions_prime[state] = temp_map;
					}
					else
					{
						std::set<State*> temp_set = transitions_prime[state][char_set.first];
						temp_set.insert(state_map.first);
						transitions_prime[state][char_set.first] = temp_set;
					}
				}
			}
		}
	}

	std::set<State*> final_states;
	for(auto state : this->m_states)
	{
		if(state->isFinal())
		{
			final_states.insert(state);
			state->setType(state_type::NONFINAL);
		}	
	}

	if(final_states.size() > 1)
	{
		State* garbage = new State(state_type::NONFINAL);

		m_states.push_back(garbage);
		std::unordered_map<char, std::set<State*>> garbage_transitions;
		garbage_transitions[0] = final_states;
		transitions_prime[garbage] = garbage_transitions;
		setInitial(garbage);
		setCurrentState(garbage);

		temp_initial->setType(state_type::FINAL);
		this->m_transitions = transitions_prime;

		removeEpsilonTransitions();
	}
	else {
		setInitial(*final_states.begin());
		setCurrentState(*final_states.begin());

		temp_initial->setType(state_type::FINAL);
		this->m_transitions = transitions_prime;
	}
}

bool Automata::isDeterminist() const
{
	// TODO

	return false;
}

bool Automata::containsEpsilonTransitions() const
{
	// TODO

	return false;
}

State* Automata::getInitial() const
{
	return this->m_initial;
}

void Automata::setInitial(State* state) 
{
	this->m_initial = state;
}
	
State* Automata::getCurrentState() const 
{
	return m_currentState;
}

void Automata::setCurrentState(int index) 
{
	m_currentState = m_states[index];
}

std::set<State*> Automata::getTransition(State* state, char transition)
{
	return m_transitions[state][transition];
}

std::unordered_map<char , std::set<State*> > Automata::getStateTransitions(State* state)
{
	return m_transitions[state];
}

State* Automata::getState(int index) const
{
	return m_states[index];
}

void Automata::addTransition(State* state, char transition, State* next) 
{
	m_transitions[state][transition].insert(next);
}

void Automata::addStateTransitions(State* state, std::unordered_map<char , std::set<State*> > transitions) 
{
	m_transitions[state] = transitions;
}

void Automata::addState(State* state) 
{
	m_states.push_back(state);
	if(!getCurrentState()){
		m_currentState = state;
		m_initial = state;
	} 
}

int Automata::nbState() const
{
	return m_states.size();
}

std::unordered_map< State*,  std::unordered_map<char , std::set<State*> > > Automata::getTransitions() const
{
	return this->m_transitions;
}

State* Automata::getStateByName(std::string name) const
{
	for(auto st : m_states)
	{
		if(st->getValue() == name)
		{
			return st;
		}
	}
	return NULL;
}


void Automata::copyTransitions(Automata* newAutomata)
{
	int nbStates = nbState();
	for(int i = 0 ;  i < nbStates ; i++)
	{		
		State* tempState = getState(i);
		copyStateTransitions(tempState, newAutomata);
	}
}

void Automata::copyStateTransitions(State* state, Automata* newAutomata)
{
	
	std::unordered_map<char, std::set<State*> > tempTransitions = m_transitions[state];
	for(auto key : tempTransitions){
		State* tempStateKey = newAutomata->getStateByName(state->getValue());
		std::set<State*> tempTransitionsWithKey = key.second;		
		for(auto st : tempTransitionsWithKey){
			State* tempStateValue = newAutomata->getStateByName(st->getValue());
			newAutomata->addTransition(tempStateKey, key.first , tempStateValue);
		}
	}
}

void Automata::setCurrentState(State* state)
{
	this->m_currentState = state;
}

std::ostream& operator<<(std::ostream& os, const Automata& automata)
{
	std::unordered_map< State*,  std::unordered_map<char , std::set<State*> > > transitions(automata.getTransitions());
	std::unordered_map<State*, std::unordered_map<char, std::set<State*>>>::iterator state_iterator;

	for(state_iterator = transitions.begin(); state_iterator != transitions.end(); ++state_iterator)
	{
		std::unordered_map<char, std::set<State*>> state_transitions(transitions[state_iterator->first]);

		std::unordered_map<char, std::set<State*>>::iterator transition_iterator;

		for(transition_iterator = state_transitions.begin(); transition_iterator != state_transitions.end(); ++transition_iterator)
		{
			std::set<State*> next_states(state_transitions[transition_iterator->first]);

			for (std::set<State*>::iterator next_states_iterator = next_states.begin(); next_states_iterator != next_states.end(); ++next_states_iterator)
				os << *(state_iterator->first) << "	- - " << transition_iterator->first << " - - >	" << **next_states_iterator << std::endl;
		}
	}

	os << "currentState: " << *(automata.getCurrentState()) << std::endl;

	return os;
}
