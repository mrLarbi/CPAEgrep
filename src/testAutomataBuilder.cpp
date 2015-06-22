#include <iostream>
#include <string>
#include "AutomataPrinter.h"
#include "AutomataBuilder.h"
#include "Automata.h"
#include "State.h"
#include "Tools.h"
#include "enum.h"

Automata* generateAutomata1(){
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

	return exAutomata;
}

Automata* generateAutomata2(){
	
	Automata* exAutomata = new Automata();

	State* state1 = Tools::makeState("4", state_type::NONFINAL);
	State* state2 = Tools::makeState("5", state_type::FINAL);

	exAutomata->addState(state1);
	exAutomata->addState(state2);

	exAutomata->addTransition(state1, 'a', state2);	
	exAutomata->addTransition(state2, 'b', state1);	

	return exAutomata;
}

Automata* generateAutomata3(){
	return new Automata();
}

void printTestHeader(std::string& title){
	std::cout << "--------------" << std::endl;
	std::cout << title << std::endl;
}

void testCaseConcat(std::string title, Automata* a1, Automata* a2){
	printTestHeader(title);	
	AutomataPrinter::printAutomata(AutomataBuilder::concat(a1, a2));
}

void testCaseKleeneStar(std::string title, Automata* a){
	printTestHeader(title);
	AutomataPrinter::printAutomata(AutomataBuilder::kleeneStar(a));
}

void testCaseAtLeastOne(std::string title, Automata* a){
	printTestHeader(title);
	AutomataPrinter::printAutomata(AutomataBuilder::atLeastOne(a));
}

void testCaseUnite(std::string title, Automata* a1, Automata* a2){
	printTestHeader(title);
	AutomataPrinter::printAutomata(AutomataBuilder::unite(a1, a2));
}

void testCaseChoice(std::string title, Automata* a){
	printTestHeader(title);
	AutomataPrinter::printAutomata(AutomataBuilder::choice(a));
}

int main(int argc, char** argv){
 
	Automata* automata1 = generateAutomata1();
	Automata* automata2 = generateAutomata2();
	Automata* automata3 = generateAutomata3();

	AutomataPrinter::printAutomata(automata1);
	AutomataPrinter::printAutomata(automata2);
	AutomataPrinter::printAutomata(automata3);

	testCaseConcat("Concat A1 and A2", automata1, automata2);
	testCaseConcat("Concat A1 and A3", automata1, automata3);

	testCaseKleeneStar("Kleene star A1", automata1);
	testCaseKleeneStar("Kleene star A2", automata2);

	testCaseAtLeastOne("At least one A1", automata1);
	testCaseAtLeastOne("At least one A2", automata2);

	testCaseUnite("Unite A1 and A2", automata1, automata2);
	testCaseUnite("Unite A1 and A3", automata1, automata3);

	testCaseChoice("Choice A1", automata1);
	testCaseChoice("Choice A2", automata2);
	
	return 0;
}
