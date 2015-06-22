#include "AutomataPrinter.h"
#include "enum.h"
#include "State.h"
#include "Tools.h"
#include <set>

void testState(std::string name, state_type::StateType type){
	AutomataPrinter::printState(Tools::makeState(name, type));
}

int main(int argc, char** argv){
	testState("1", state_type::NONFINAL);
	testState("2", state_type::FINAL);
	testState("3", state_type::NONFINAL);
	testState("4", state_type::FINAL);
	testState("5", state_type::FINAL);
	testState("6", state_type::NONFINAL);
	testState("7", state_type::NONFINAL);
	testState("8", state_type::FINAL);
	testState("9", state_type::FINAL);

	State* yolo1 = new State("12", state_type::FINAL);
	State* yolo2 = new State("13", state_type::FINAL);
	std::set<State*> yoloStates = {yolo1, yolo2};
	State* finalYolo = new State(yoloStates);
	AutomataPrinter::printState(finalYolo);
}
