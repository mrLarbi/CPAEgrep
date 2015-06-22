#include <iostream>

#include "Automata.h"
#include "Automata.h"
#include "AutomataBuilder.h"
#include "enum.h"
#include "State.h"

Automata* AutomataBuilder::concat(Automata* a1, Automata* a2){
	
	int nbState1 = a1->nbState();	
	int nbState2 = a2->nbState();
	int nbStates = nbState1 + nbState2;	

	if(!nbState1){
		return a2;
	}
	
	if(!nbState2){
		return a1;
	}

	State* initialA1 = new State(a1->getInitial());	
	State* initialA2 = new State(a2->getInitial());
	
	Automata* newAutomata = new Automata();

	for(int i = 0 ; i < nbStates ; i++){
		State* tempState;
		State* tempOriginalState = (i < nbState1 ? a1->getState(i) : a2->getState(i-nbState1));
		if(tempOriginalState == a1->getInitial())
		{
			tempState = initialA1;
		}
		else if (tempOriginalState == a2->getInitial())
		{
			tempState = initialA2;
		}
		else{
			tempState = new State(tempOriginalState);
		}		
		newAutomata->addState(tempState);
		if(tempState->isFinal() && i < nbState1){
			newAutomata->addTransition(tempState, 0, initialA2);
			tempState->setType(state_type::NONFINAL);
		}
	}

	a1->copyTransitions(newAutomata);
	a2->copyTransitions(newAutomata);

	newAutomata->setCurrentState(0);

	for(int i = 0 ; i < nbStates ; i++){
		State* tempState = newAutomata->getState(i);
		tempState->updateValue(); 
	}

	return newAutomata;	
}

Automata* AutomataBuilder::weakerConcat(Automata* a1, Automata* a2)
{
	int nbState1 = a1->nbState();	
	int nbState2 = a2->nbState();
	int nbStates = nbState1 + nbState2;	

	if(!nbState1){
		return a2;
	}
	
	if(!nbState2){
		return a1;
	}

	State* initialA1 = new State(a1->getInitial());	
	State* initialA2 = new State(a2->getInitial());
	
	Automata* newAutomata = new Automata();	
	newAutomata->setInitial(initialA1);

	for(int i = 0 ; i < nbStates ; i++){
		State* tempOriginalState = (i < nbState1 ? a1->getState(i) : a2->getState(i-nbState1));
		State* tempState;
		if(tempOriginalState == a1->getInitial())
		{
			tempState = initialA1;
		}
		else if (tempOriginalState == a2->getInitial())
		{
			tempState = initialA2;
		}
		else
		{
			tempState = new State(tempOriginalState);
		} 		
		newAutomata->addState(tempState);
		if(tempState->isFinal() && i < nbState1){
			newAutomata->addTransition(tempState, 0, initialA2);
		}
	}

	a1->copyTransitions(newAutomata);
	a2->copyTransitions(newAutomata);

	newAutomata->setCurrentState(0);

	for(int i = 0 ; i < nbStates ; i++){
		State* tempState = newAutomata->getState(i);
		tempState->updateValue(); 
	}

	return newAutomata;		
}

Automata* AutomataBuilder::kleeneStar(Automata* a){
	int nbStateA = a->nbState();	
	Automata* newAutomata = new Automata();
	State* initialA = new State(a->getInitial());
	newAutomata->setInitial(initialA);
	for(int i = 0 ; i < nbStateA ; i++){
		State* tempOriginalState = a->getState(i);
		State* tempState;
		if(tempOriginalState == a->getInitial())
		{
			tempState = initialA;
			tempState->setType(state_type::FINAL);
		}
		else
		{
			tempState = new State(tempOriginalState);
		}
		newAutomata->addState(tempState);
		if(tempState->isFinal()){
			newAutomata->addTransition(tempState, 0, newAutomata->getInitial());		
		}
	}

	a->copyTransitions(newAutomata);

	for(int i = 0 ; i < nbStateA ; i++){
		State* tempState = newAutomata->getState(i);
		tempState->updateValue(); 
	}
	
	return newAutomata;
}

Automata* AutomataBuilder::atLeastOne(Automata* a){
	Automata* newAutomata = concat(a, kleeneStar(a));
	return newAutomata;
}

Automata* AutomataBuilder::unite(Automata* a1, Automata* a2)
{
	int nbState1 = a1->nbState();	
	int nbState2 = a2->nbState();
	
	if(!nbState1){
		return a2;
	}
	
	if(!nbState2){
		return a1;
	}

	int nbStates = nbState1 + nbState2;	
	State* newInitial = new State(state_type::NONFINAL);
	
	Automata* newAutomata = new Automata();
	newAutomata->addState(newInitial);
	newAutomata->setInitial(newInitial);
	newAutomata->setCurrentState(0);
	
	State* initialA1 = new State(a1->getInitial());
	State* initialA2 = new State(a2->getInitial());
	
	for(int i = 0 ; i < nbState1 + nbState2 ; i++){
		State* tempOriginalState = (i < nbState1 ? a1->getState(i) : a2->getState(i-nbState1));
		State* tempState;
		if(tempOriginalState == a1->getInitial())
		{
			tempState = initialA1;
		}
		else if (tempOriginalState == a2->getInitial())
		{
			tempState = initialA2;
		}
		else
		{
			tempState = new State(tempOriginalState);
		}
		newAutomata->addState(tempState);
	}

	a1->copyTransitions(newAutomata);
	a2->copyTransitions(newAutomata);

	newAutomata->addTransition(newInitial, 0, initialA1);
	newAutomata->addTransition(newInitial, 0, initialA2);

	for(int i = 0 ; i < nbStates + 1; i++){
		State* tempState = newAutomata->getState(i);
		tempState->updateValue(); 
	}

	return newAutomata;
}

Automata* AutomataBuilder::uniteChars(std::vector<char>& chars)
{
	int charSize = chars.size();
	if(!charSize)
	{
		return new Automata();
	}
	Automata* newAutomata = new Automata();
	State* newInitial = new State(state_type::NONFINAL);
	newAutomata->addState(newInitial);
	for ( int i = 0; i < charSize; i++ )
	{
		State* tempInitial = new State(state_type::NONFINAL);
		State* tempFinal = new State(state_type::FINAL);
		newAutomata->addState(tempInitial);
		newAutomata->addState(tempFinal);
		newAutomata->addTransition(tempInitial, chars[i] , tempFinal);
		newAutomata->addTransition(newInitial, 0, tempInitial);
	}
	return newAutomata;
}

Automata* AutomataBuilder::choice(Automata* a)
{
	
	State* nullFinalState = new State(state_type::FINAL);

	Automata* newAutomata = new Automata();
	newAutomata->addState(nullFinalState);
	newAutomata->setCurrentState(0);
	
	return unite(a, newAutomata);		
}

Automata* AutomataBuilder::atLeastN(Automata* a , int n)
{
	Automata* newAutomata = new Automata();
	for ( int i = 0; i < n - 1 ; i ++ ) {
		newAutomata = concat(newAutomata, a);
	}
	return concat(newAutomata, kleeneStar(a));
}

Automata* AutomataBuilder::atMostM(Automata* a , int m)
{
	Automata* newAutomata = new Automata();
	State* newInitial = new State(state_type::FINAL);
	newAutomata->addState(newInitial);
	for ( int i = 0; i < m; i ++ ) {
		newAutomata = weakerConcat(newAutomata, a);
	}
	return newAutomata;
}
		
Automata* AutomataBuilder::allWords()
{
	Automata* newAutomata = new Automata();
	State* newInitial = new State(state_type::FINAL);
	newAutomata->addState(newInitial);
	for ( int i = 0; i < 128; i++ )
	{
		newAutomata->addTransition(newInitial, i, newInitial);
	}
	return newAutomata;
}

Automata* AutomataBuilder::oneLetter(char a)
{
	Automata* newAutomata = new Automata();
	State* newInitial = new State(state_type::NONFINAL);
	State* newFinal = new State(state_type::FINAL);
	newAutomata->addState(newInitial);
	newAutomata->addState(newFinal);
	newAutomata->addTransition(newInitial, a, newFinal);
	return newAutomata;
}

Automata* AutomataBuilder::allLetters()
{
	Automata* newAutomata = new Automata();
	State* newInitial = new State(state_type::NONFINAL);
	State* newFinal = new State(state_type::FINAL);
	newAutomata->addState(newInitial);
	newAutomata->addState(newFinal);
	for ( int i = 0; i < 128 ; i++ )
	{
		newAutomata->addTransition(newInitial, i, newFinal);
	}
	return newAutomata;
}

Automata* AutomataBuilder::startsWith(Automata* a)
{
	return concat(a, allWords());
}

Automata* AutomataBuilder::endsWith(Automata* a)
{
	return concat(allWords(), a);
}

Automata* AutomataBuilder::betweenNM(Automata* a, int n, int m)
{
	return weakerConcat(atLeastN(a,n), atMostM(a, m-n));
}

