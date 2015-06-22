#include "AutomataPrinter.h"
#include "Automata.h"
#include "Tools.h"

int main(int argc, char** argv){
	
	Automata* exAutomata = new Automata();
		
	State* state1 = Tools::makeState("1", state_type::NONFINAL);
	State* state2 = Tools::makeState("2", state_type::NONFINAL);
	State* state3 = Tools::makeState("3", state_type::FINAL);

	exAutomata->addState(state1);
	exAutomata->addState(state2);
	exAutomata->addState(state3);
	exAutomata->addTransition(state1, 'a', state2);	
	exAutomata->addTransition(state2, 'b', state3);	
	exAutomata->addTransition(state3, 'a', state3);	
	exAutomata->addTransition(state3, 'b', state3);	
	
	AutomataPrinter::printAutomata(exAutomata);
	




}
