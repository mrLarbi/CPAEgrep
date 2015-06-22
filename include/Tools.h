#ifndef H_TOOLS
#define H_TOOLS

#include <string>
#include "enum.h"
#define NB_CHARACTERS 128


typedef enum {DUPL_STAR, DUPL_CHOICE, DUPL_PLUS, DUPL_MIN_N, DUPL_MAX_M, DUPL_BET_N_M} DuplType;
	
typedef struct{
	DuplType type;
	int value1;
	int value2;
}dupl_t;

class State;
class Automata;

class Tools{
	public :
		static State* makeState(std::string name, state_type::StateType type);
};

int* make128zeros();
void resetBracketList(int* blist);	
void prepareAutomata(Automata* automata);
Automata* makeDuplAutomata(Automata* a, dupl_t dupl);
void updateDupl(dupl_t* dupl, DuplType t, int v1, int v2);
void startAnalysis(Automata* automata);
void dealwith_colon_bracket(int* blist, char* content);
void dealwith_equal_bracket_single(int* blist, char content);
void dealwith_equal_bracket_multi(int* blist, char* content);
void dealwith_dot_bracket_single(int* blist, char content);
void dealwith_dot_bracket_multi(int* blist, char* content);
void dealwith_dot_bracket_meta(int* blist, char content);
void dealwith_range(char a, char b, int* blist);
void invertBracketList(int* blist);
Automata* makeAutomata_withBracketList(int* blist);
void activateDigits(int* blist);
void activateUppers(int* blist);
void activateLowers(int* blist);
void activateAlpha(int* blist);
void activateAlnum(int* blist);
void activatePuncts(int* blist);
void activateHexa(int* blist);
void activateSpace(int* blist);
void activateTab(int* blist);
void activateControls(int* blist);
void activateGraph(int* blist);
void activateSpaces(int* blist);
void activatePrint(int* blist);
void activateBlank(int* blist);

#endif
