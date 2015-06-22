#include <iostream>

#include "State.h"


unsigned long int State::CURRENT_STATE_NUMBER;

State::State(std::string val, state_type::StateType type)
{
	this->m_value = val;
	this->m_type = type;
}

State::State(state_type::StateType type)
{
	updateValue();
	this->m_type = type;
}

State::State(std::set<State*> states)
{
	std::string value = "";
	state_type::StateType type = state_type::NONFINAL;

	int size = states.size();

	if(size > 1)
	{
		int i = 0;

		for(std::set<State*> ::iterator it = states.begin(); it != states.end(); ++it)
		{
			if(i < (size - 1))
			{
				value.append((*it)->getValue()+"_");

				if((*it)->isFinal())
					type = state_type::FINAL;
				i++;
			}
			else
			{
				value.append((*it)->getValue());

				if((*it)->isFinal())
					type = state_type::FINAL;
				i++;
			}
		}
	}
	else
	{
		value = (*(states.begin()))->getValue();
		type = (*(states.begin()))->getType();
	}

	this->m_value = value;
	this->m_type = type;
}

State::State(State* state)
{
	this->m_value = state->m_value;
	this->m_type = state->m_type;
}

State::~State(){}

bool State::isFinal() const
{
	return this->m_type == state_type::FINAL;
}

std::string State::getValue() const
{
	return this->m_value;
}

state_type::StateType State::getType() const
{
	return this->m_type;
}

void State::setType(state_type::StateType type)
{
	this->m_type = type;
}

void State::updateValue()
{
	this->m_value = std::to_string(CURRENT_STATE_NUMBER++);
}

std::ostream& operator<<(std::ostream& os, const State& state)
{
    os << "{ " << state.getValue() << ", " << state_type::statesNames[state.getType()] << ", @:" << &state << " }";
    return os;
}
