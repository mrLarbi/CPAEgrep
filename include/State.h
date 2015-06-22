#ifndef H_STATE
#define H_STATE

#include <iostream>
#include <set>
#include <string>

#include "enum.h"

class State{

	private:
		std::string m_value;
		state_type::StateType m_type;

		static unsigned long int CURRENT_STATE_NUMBER;
	
	public:
		State(std::string val, state_type::StateType type);
		State(state_type::StateType type);
		State(std::set<State*> states);
		State(State* state);
		~State();
		bool isFinal() const;
		std::string getValue() const;
		state_type::StateType getType() const;
		void setType(state_type::StateType type);
		void updateValue();
		friend std::ostream& operator<<(std::ostream& os, const State& state);
	

};

#endif
