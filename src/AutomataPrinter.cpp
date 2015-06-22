#include <iostream>
#include <unordered_map>

#include "AutomataPrinter.h"
#include "Automata.h"
#include "State.h"

int AutomataPrinter::nbState;

void AutomataPrinter::printAutomata(Automata* automata){
	printAutomataStates(automata);
	printCurrentState(automata);
	printInitialState(automata);
	printAutomataTransitions(automata);
	printAutomataDeterminism(automata);
}

void AutomataPrinter::printAutomataStates(Automata* automata){
	nbState = automata->nbState();
	std::cout << "There are " << nbState << " states : " << std::endl;
	for(int i = 0 ; i < nbState ; i++){
		std::cout << "\t- ";
		printState(automata->getState(i));
	}
}

void AutomataPrinter::printCurrentState(Automata* automata){
	State* automataCurrentState = automata->getCurrentState();			
	if(automataCurrentState){
		std::cout << "Current state is : " << automataCurrentState->getValue() << std::endl;	
	}
}

void AutomataPrinter::printInitialState(Automata* automata){
	State* tempInitial = automata->getInitial();	
	if(tempInitial){
		std::cout << "Initial state is : " << tempInitial->getValue() << std::endl;	
	}
}

void AutomataPrinter::printAutomataTransitions(Automata* automata){
	std::cout << "Transitions : " << std::endl;	
	for(int i = 0 ; i < nbState ; i++){
		printTransition(automata , automata->getState(i));
	}
}

void AutomataPrinter::printAutomataDeterminism(Automata* automata){
	std::cout << "This automata is " << (automata->isDeterminist() ? " " : "not ") << "determinist" << std::endl;
}

void AutomataPrinter::printTransition(Automata* automata, State* state){
	std::unordered_map<char, std::set<State*> >	transitions = automata->getStateTransitions(state);
	for(auto it : transitions){
		char key = it.first;
		std::set<State*> value = it.second;
		for(auto st : value){
			std::cout << "\t- ";
			std::cout << state->getValue() << "->(" << key << ")" << st->getValue() << std::endl;
		}
	}
}

void AutomataPrinter::printState(State* state){
	std::cout << "State : " << state->getValue() << " , " << (state->isFinal() ? "Final" : "Non Final") << std::endl;
}
