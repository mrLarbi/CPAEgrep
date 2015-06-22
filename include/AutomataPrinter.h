#ifndef H_AUTOMATAPRINTER
#define H_AUTOMATAPRINTER

class Automata;
class State;


class AutomataPrinter {
	public :
		
		static void printAutomata(Automata* automata);
		static void printState(State* state);
	private :
		static int nbState;
		static void printAutomataStates(Automata* automata);
		static void printCurrentState(Automata* automata);
		static void printInitialState(Automata* automata);
		static void printAutomataTransitions(Automata* automata);
		static void printAutomataDeterminism(Automata* automata);

		static void printTransition(Automata* automata, State* state);
		
};

#endif
