#pragma once
#include "deterministic_finite_state_automata.h"
class non_deterministic_finite_state_automata: public finite_state_automata
{
public:
	using finite_state_automata::finite_state_automata;
	bool run(std::deque<int> s) const override;
private:
};

using NFA = non_deterministic_finite_state_automata;

