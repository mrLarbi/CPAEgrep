#ifndef H_AUTOMATABUILDER
#define H_AUTOMATABUILDER

#include <vector>

class Automata;

class AutomataBuilder{

	public:
		static Automata* concat(Automata* a1, Automata* a2);
		static Automata* weakerConcat(Automata* a1, Automata* a2);
		static Automata* kleeneStar(Automata* a);
		static Automata* atLeastOne(Automata* a);
		static Automata* unite(Automata* a1, Automata* a2);
		static Automata* uniteChars(std::vector<char>& chars);		
		static Automata* choice(Automata* a);
		static Automata* atLeastN(Automata* a, int n);
		static Automata* atMostM(Automata* a, int m);
		static Automata* allWords();
		static Automata* allLetters();
		static Automata* oneLetter(char a);
		static Automata* startsWith(Automata* a);
		static Automata* endsWith(Automata* a);
		static Automata* betweenNM(Automata* a, int n, int m);
};

#endif
