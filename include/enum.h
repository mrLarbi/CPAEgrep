#ifndef H_ENUMS
#define H_ENUMS

#include <string>

namespace state_type {
	enum StateType { NONFINAL = 0, FINAL, ENUM_SIZE };
	static const std::string statesNames[ENUM_SIZE] = { "NONFINAL", "FINAL" };
}

#endif
