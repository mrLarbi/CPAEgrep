#include <chrono>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

#include "State.h"
#include "Automata.h"
#include "Automata.h"

std::string path = "./automata.dot";

void test_determinisation()
{
	std::cout << "Testing determinisation..." << std::endl << std::endl;

	State zero("0", state_type::NONFINAL);
	State one("1", state_type::NONFINAL);
	State two("2", state_type::NONFINAL);
	State three("3", state_type::FINAL);

	std::vector<State*> states;

	std::set<State*> zeroStatesA;
	std::set<State*> zeroStatesB;
	std::set<State*> oneStatesA;
	std::set<State*> twoStatesB;

	states.push_back(&zero);
	states.push_back(&one);
	states.push_back(&two);
	states.push_back(&three);

	zeroStatesA.insert(&zero);
	zeroStatesB.insert(&zero);
	zeroStatesB.insert(&one);
	oneStatesA.insert(&two);
	twoStatesB.insert(&three);

	std::unordered_map<char , std::set<State*> > zeroTransitions;
	std::unordered_map<char , std::set<State*> > oneTransitions;
	std::unordered_map<char , std::set<State*> > twoTransitions;
	std::unordered_map<char , std::set<State*> > threeTransitions;

	zeroTransitions['a'] = zeroStatesA;
	zeroTransitions['b'] = zeroStatesB;
	oneTransitions['a'] = oneStatesA;
	twoTransitions['b'] = twoStatesB;

	std::unordered_map< State*,  std::unordered_map<char , std::set<State*> > > transitions;

	transitions[&zero] = zeroTransitions;
	transitions[&one] = oneTransitions;
	transitions[&two] = twoTransitions;
	transitions[&three] = threeTransitions;

	Automata automata(transitions);
	automata.setInitial(&zero);
	automata.setCurrentState(&zero);
	automata.setStates(states);

	std::cout << "Before:" << std::endl << automata << std::endl;

	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	automata.determinise();
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

	std::cout << "After: " << std::endl << automata << std::endl;
	
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
	std::cout << "Execution time: " << duration << " milliseconds." << std::endl << std::endl;
}

void test_epsilon_deleting()
{
	std::cout << "Testing epsilon deleting..." << std::endl << std::endl;

	State zero("0", state_type::NONFINAL);
	State one("1", state_type::NONFINAL);
	State two("2", state_type::FINAL);
	State three("3", state_type::NONFINAL);
	State four("4", state_type::FINAL);
	State five("5", state_type::NONFINAL);

	std::vector<State*> states;

	std::set<State*> zeroStatesEps;
	std::set<State*> oneStatesA;
	std::set<State*> twoStatesEps;
	std::set<State*> twoStatesA;
	std::set<State*> threeStatesEps;
	std::set<State*> fourStates;
	std::set<State*> fiveStatesEps;
	std::set<State*> fiveStatesB;

	states.push_back(&zero);
	states.push_back(&one);
	states.push_back(&two);
	states.push_back(&three);
	states.push_back(&four);
	states.push_back(&five);

	zeroStatesEps.insert(&one);
	zeroStatesEps.insert(&two);
	zeroStatesEps.insert(&three);
	oneStatesA.insert(&four);
	twoStatesEps.insert(&four);
	twoStatesA.insert(&five);
	threeStatesEps.insert(&five);
	fiveStatesEps.insert(&three);
	fiveStatesB.insert(&four);

	std::unordered_map<char , std::set<State*> > zeroTransitions;
	std::unordered_map<char , std::set<State*> > oneTransitions;
	std::unordered_map<char , std::set<State*> > twoTransitions;
	std::unordered_map<char , std::set<State*> > threeTransitions;
	std::unordered_map<char , std::set<State*> > fourTransitions;
	std::unordered_map<char , std::set<State*> > fiveTransitions;

	zeroTransitions[0] = zeroStatesEps;
	oneTransitions['a'] = oneStatesA;
	twoTransitions[0] = twoStatesEps;
	twoTransitions['a'] = twoStatesA;
	threeTransitions[0] = threeStatesEps;
	fiveTransitions[0] = fiveStatesEps;
	fiveTransitions['b'] = fiveStatesB;

	std::unordered_map< State*,  std::unordered_map<char , std::set<State*> > > transitions;

	transitions[&zero] = zeroTransitions;
	transitions[&one] = oneTransitions;
	transitions[&two] = twoTransitions;
	transitions[&three] = threeTransitions;
	transitions[&four] = fourTransitions;
	transitions[&five] = fiveTransitions;

	Automata automata(transitions);
	automata.setInitial(&zero);
	automata.setStates(states);

	std::cout << "Before:" << std::endl << automata << std::endl;

	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	
	automata.removeEpsilonTransitions();

	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

	std::cout << "After: " << std::endl << automata << std::endl;
	
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
	std::cout << "Execution time: " << duration << " milliseconds." << std::endl << std::endl;
}

void test_parsing()
{
	std::cout << "Testing parsing..." << std::endl << std::endl;

	State zero("0", state_type::NONFINAL);
	State one("1", state_type::NONFINAL);
	State two("2", state_type::NONFINAL);
	State three("3", state_type::FINAL);

	std::vector<State*> states;

	std::set<State*> zeroStatesA;
	std::set<State*> zeroStatesB;
	std::set<State*> oneStatesA;
	std::set<State*> twoStatesB;

	states.push_back(&zero);
	states.push_back(&one);
	states.push_back(&two);
	states.push_back(&three);

	zeroStatesA.insert(&zero);
	zeroStatesB.insert(&zero);
	zeroStatesB.insert(&one);
	oneStatesA.insert(&two);
	twoStatesB.insert(&three);

	std::unordered_map<char , std::set<State*> > zeroTransitions;
	std::unordered_map<char , std::set<State*> > oneTransitions;
	std::unordered_map<char , std::set<State*> > twoTransitions;
	std::unordered_map<char , std::set<State*> > threeTransitions;

	zeroTransitions['a'] = zeroStatesA;
	zeroTransitions['b'] = zeroStatesB;
	oneTransitions['a'] = oneStatesA;
	twoTransitions['b'] = twoStatesB;

	std::unordered_map< State*,  std::unordered_map<char , std::set<State*> > > transitions;

	transitions[&zero] = zeroTransitions;
	transitions[&one] = oneTransitions;
	transitions[&two] = twoTransitions;
	transitions[&three] = threeTransitions;

	Automata automata(transitions);
	automata.setInitial(&zero);
	automata.setCurrentState(&zero);
	automata.setStates(states);

	std::cout << "Avant: " << std::endl << automata << std::endl;
	automata.determinise();
	std::cout << "après: " << std::endl << automata << std::endl;
	automata.minimise();
	std::cout << "après après: " << std::endl << automata << std::endl;

	std::set<std::string> words;
	words.insert("bab");
	words.insert("ba");
	words.insert("");
	words.insert("aaaaaabbbab");
	words.insert("bbbb");

	for(auto word : words)
	{
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
		std::cout << "\"" << word << "\"";
		std::cout << " accepted ? --> " << automata.isAcceptedWord(word) << std::endl;
		std::cout << "word: ";
		std::cout << word << std::endl;
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
		std::cout << "Execution time: " << duration << " milliseconds." << std::endl << std::endl;
	}
}

void test_parsing_2()
{
	std::cout << "Testing parsing 2..." << std::endl << std::endl;

	State zero("0", state_type::NONFINAL);
	State one("1", state_type::NONFINAL);
	State two("2", state_type::NONFINAL);
	State three("3", state_type::FINAL);

	std::vector<State*> states;

	std::set<State*> zeroStatesA;
	std::set<State*> zeroStatesB;
	std::set<State*> oneStatesA;
	std::set<State*> twoStatesB;

	states.push_back(&zero);
	states.push_back(&one);
	states.push_back(&two);
	states.push_back(&three);

	zeroStatesA.insert(&zero);
	zeroStatesB.insert(&zero);
	zeroStatesB.insert(&one);
	oneStatesA.insert(&two);
	twoStatesB.insert(&three);

	std::unordered_map<char , std::set<State*> > zeroTransitions;
	std::unordered_map<char , std::set<State*> > oneTransitions;
	std::unordered_map<char , std::set<State*> > twoTransitions;
	std::unordered_map<char , std::set<State*> > threeTransitions;

	zeroTransitions['a'] = zeroStatesA;
	zeroTransitions['b'] = zeroStatesB;
	oneTransitions['a'] = oneStatesA;
	twoTransitions['b'] = twoStatesB;

	std::unordered_map< State*,  std::unordered_map<char , std::set<State*> > > transitions;

	transitions[&zero] = zeroTransitions;
	transitions[&one] = oneTransitions;
	transitions[&two] = twoTransitions;
	transitions[&three] = threeTransitions;

	Automata automata(transitions);
	automata.setInitial(&zero);
	automata.setCurrentState(&zero);
	automata.setStates(states);

	std::cout << "Avant: " << std::endl << automata << std::endl;
	automata.determinise();
	std::cout << "après: " << std::endl << automata << std::endl;
	automata.minimise();
	std::cout << "après après: " << std::endl << automata << std::endl;

	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

	std::string test_file = "./data/test";
	
	automata.parse_file(test_file);

	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
	std::cout << "Execution time: " << duration << " milliseconds." << std::endl << std::endl;
}

void test_serialize()
{
	std::cout << "Testing serialize..." << std::endl << std::endl;

	State zero("0", state_type::NONFINAL);
	State one("1", state_type::NONFINAL);
	State two("2", state_type::NONFINAL);
	State three("3", state_type::FINAL);

	std::vector<State*> states;

	std::set<State*> zeroStatesA;
	std::set<State*> zeroStatesB;
	std::set<State*> oneStatesA;
	std::set<State*> twoStatesB;

	states.push_back(&zero);
	states.push_back(&one);
	states.push_back(&two);
	states.push_back(&three);

	zeroStatesA.insert(&zero);
	zeroStatesB.insert(&zero);
	zeroStatesB.insert(&one);
	oneStatesA.insert(&two);
	twoStatesB.insert(&three);

	std::unordered_map<char , std::set<State*> > zeroTransitions;
	std::unordered_map<char , std::set<State*> > oneTransitions;
	std::unordered_map<char , std::set<State*> > twoTransitions;
	std::unordered_map<char , std::set<State*> > threeTransitions;

	zeroTransitions['a'] = zeroStatesA;
	zeroTransitions['b'] = zeroStatesB;
	oneTransitions['a'] = oneStatesA;
	twoTransitions['b'] = twoStatesB;

	std::unordered_map< State*,  std::unordered_map<char , std::set<State*> > > transitions;

	transitions[&zero] = zeroTransitions;
	transitions[&one] = oneTransitions;
	transitions[&two] = twoTransitions;
	transitions[&three] = threeTransitions;

	Automata automata(transitions);
	automata.setInitial(&zero);
	automata.setCurrentState(&zero);
	automata.setStates(states);

	std::cout << "Avant: " << std::endl << automata << std::endl;
	automata.determinise();
	std::cout << "après: " << std::endl << automata << std::endl;
	automata.minimise();
	std::cout << "après après: " << std::endl << automata << std::endl;

	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

	automata.serialize(path);

	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
	std::cout << "Execution time: " << duration << " milliseconds." << std::endl << std::endl;
}



int main(int argc, char** argv)
{
	test_determinisation();
	test_epsilon_deleting();
	test_parsing();
	test_parsing_2();
	test_serialize();
}
