#pragma once
#include "finite_state_automata.h"
class deterministic_finite_state_automata: public finite_state_automata
{
public:
	using finite_state_automata::finite_state_automata;
	bool execute(std::deque<int>& s) const override;
};

using DFA = deterministic_finite_state_automata;
