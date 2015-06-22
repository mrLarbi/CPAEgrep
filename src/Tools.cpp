#include <iostream>
#include <string.h>

#include "Tools.h"
#include "State.h"
#include "Automata.h"
#include "Automata.h"
#include "AutomataBuilder.h"
#include "FileList.h"

State* Tools::makeState(std::string name, state_type::StateType type)
{
	return new State(name, type);
}

int* make128zeros()
{
	int i = 0;
	int* temp = (int*)malloc(sizeof(int)*NB_CHARACTERS);
	for( ; i < NB_CHARACTERS ; i++){
		temp[i] = 0;
	}
	return temp;
}

void resetBracketList(int* blist)
{
	for ( int i = 0; i < NB_CHARACTERS; i++ )
	{
		blist[i] = 0;
	}
}

void prepareAutomata(Automata* automata)
{
	std::string path = "./automata_initial.dot";
	automata->serialize(path);
	automata->removeEpsilonTransitions();
	path = "./automata_epsilon.dot";
	automata->serialize(path);
	automata->determinise();
	path = "./automata_determinise.dot";
	automata->serialize(path);
	automata->minimise();
	path = "./automata_minimise.dot";
	automata->serialize(path);
}
	
Automata* makeDuplAutomata(Automata* a, dupl_t dupl)
{
	Automata* newAutomata = new Automata();
	switch(dupl.type){
		case DUPL_STAR:
			newAutomata = AutomataBuilder::kleeneStar(a);
			break;
		case DUPL_PLUS:
			newAutomata = AutomataBuilder::atLeastOne(a);
			break;
		case DUPL_CHOICE:
			newAutomata = AutomataBuilder::choice(a);
			break;
		case DUPL_MIN_N:
			newAutomata = AutomataBuilder::atLeastN(a, dupl.value1);
			break;
		case DUPL_MAX_M:
			newAutomata = AutomataBuilder::atMostM(a, dupl.value2);
			break;
		case DUPL_BET_N_M:
			newAutomata = AutomataBuilder::betweenNM(a, dupl.value1, dupl.value2);
			break;
	}
	return newAutomata;
}
	
void updateDupl(dupl_t* dupl, DuplType t, int v1, int v2)
{
	dupl->type = t;
	dupl->value1 = v1;
	dupl->value2 = v2;
}

void startAnalysis(Automata* automata)
{
	prepareAutomata(automata); 
	FileList* filelist = FileList::getFileListInstance();
	
	if(filelist->nbFiles())
	{
		for ( int i = 0; i < filelist->nbFiles(); i++ )
		{
			std::string filename = filelist->getFileName(i);
			automata->parse_file(filename);
		}	
	}
	else
	{
		while(1)
		{
			std::string text;
			std::cin >> text ;
			std::cout << text << ": " << automata->isAcceptedWord(text) << std::endl;
		}
	}
	
}

void activateDigits(int* blist)
{
	for ( int i = 48; i < 58; i++ )
	{
		blist[i] = 1;
	}
}

void activateUppers(int* blist)
{
	for ( int i = 65; i < 91; i++ )
	{
		blist[i] = 1;
	}
}

void activateLowers(int* blist)
{
	for ( int i = 97; i < 123; i++ )
	{
		blist[i] = 1;
	}
}

void activateAlpha(int* blist)
{
	activateLowers(blist);
	activateUppers(blist);
}

void activateAlnum(int* blist)
{
	activateAlpha(blist);
	activateDigits(blist);
}

void activatePuncts(int* blist)
{
	for ( int i = 33; i < 48; i++ )
	{
		blist[i] = 1;
	}
	for ( int i = 58; i < 65; i++ )
	{
		blist[i] = 1;
	}
	for ( int i = 91; i < 97; i++ )
	{
		blist[i] = 1;
	}
	for ( int i = 123; i < 127; i++ )
	{
		blist[i] = 1;
	}
}

void activateHexa(int* blist)
{
	activateDigits(blist);
	for ( int i = 65; i < 71; i++ )
	{
		blist[i] = 1;
	}
	for ( int i = 97; i < 103; i++ )
	{
		blist[i] = 1;
	}
}

void activateSpace(int* blist)
{
	blist[32] = 1;
}

void activateTab(int* blist)
{
	blist[9] = 1;
}

void activateControls(int* blist)
{
	for ( int i = 0; i < 38; i++ )
	{
		blist[i] = 1;
	}
	blist[177] = 1;
}

void activateGraph(int* blist)
{
	activateAlnum(blist);
	activatePuncts(blist);
}

void activateSpaces(int* blist)
{
	activateSpace(blist);
	activateTab(blist);
	for ( int i = 10; i < 14; i++ )
	{
		blist[i] = 1;
	}
}

void activatePrint(int* blist)
{
	activateGraph(blist);
	activateSpace(blist);
}

void activateBlank(int* blist)
{
	activateSpace(blist);
	activateTab(blist);
}

void dealwith_colon_bracket(int* blist, char* content)
{
	if(!strcmp(content, "alnum"))
	{
		activateAlnum(blist);
	}
	else if (!strcmp(content, "alpha"))
	{
		activateAlpha(blist);
	}
	else if (!strcmp(content, "blank"))
	{
		activateBlank(blist);
	}
	else if (!strcmp(content, "cntrl"))
	{
		activateControls(blist);
	}
	else if (!strcmp(content, "digit"))
	{
		activateDigits(blist);
	}
	else if (!strcmp(content, "graph"))
	{
		activateGraph(blist);
	}
	else if (!strcmp(content, "lower"))
	{
		activateLowers(blist);
	}
	else if (!strcmp(content, "print"))
	{
		activatePrint(blist);
	}
	else if (!strcmp(content, "punct"))
	{
		activatePuncts(blist);
	}
	else if (!strcmp(content, "space"))
	{
		activateSpaces(blist);
	}
	else if (!strcmp(content, "upper"))
	{
		activateUppers(blist);
	}
	else if (!strcmp(content, "xdigit"))
	{
		activateHexa(blist);
	}
	
}

void dealwith_equal_bracket_single(int* blist, char content)
{
	std::cerr << "Not implemented yet" << std::endl;
}

void dealwith_equal_bracket_multi(int* blist, char* content)
{
	std::cerr << "Not implemented yet" << std::endl;
}

void dealwith_dot_bracket_single(int* blist, char content)
{
	std::cerr << "Not implemented yet" << std::endl;
}

void dealwith_dot_bracket_multi(int* blist, char* content)
{
	std::cerr << "Not implemented yet" << std::endl;
}
	
void dealwith_dot_bracket_meta(int* blist, char content)
{
	std::cerr << "Not implemented yet" << std::endl;
}

void dealwith_range(char a, char b, int* blist)
{
	if (b < a)
	{
		std::cerr << "Invalide range expression" << std::endl;
		exit(1);
	}
	for ( int i = (int)a; i <= b; i++ )
	{
		blist[i] = 1;
	}
}

void invertBracketList(int* blist)
{
	for ( int i = 0; i < NB_CHARACTERS; i++)
	{
		blist[i] = (blist[i] ? 0 : 1);
	}
}

Automata* makeAutomata_withBracketList(int* blist)
{
	std::vector<char> activatedChars;
	for ( int i = 0; i < NB_CHARACTERS; i++ )
	{
		if (blist[i])
		{
			activatedChars.push_back(i);
		}
	}
	return AutomataBuilder::uniteChars(activatedChars);
}
