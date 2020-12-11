#pragma once
#include "finite_state_automata.h"

/*
* This class is the class of deterministic finite state automata
* A deterministic finite state machine is a FSA with the additional property that every transition is uniquely determined by its
* source and label
* 
* The language of a 
*/

class deterministic_finite_state_automata: public finite_state_automata
{
public:
	using finite_state_automata::finite_state_automata;
	bool execute(std::deque<int>& s) const override;
};

using DFA = deterministic_finite_state_automata;
