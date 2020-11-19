#pragma once
#include "non_deterministic_finite_state_automata.h"
class epsilon_non_deterministic_finite_state_automata : public finite_state_automata
{
	public:
	using finite_state_automata::finite_state_automata;
	bool run(std::deque<int> S) const override;
	void add_transition(int i, int j, int c) override;
	void add_transition(int i, int j);
	static constexpr int ε = -1;
private:
	non_deterministic_finite_state_automata equivalent_NFA() const;
	epsilon_non_deterministic_finite_state_automata reverse() const;
	std::set<int> acceptable_states() const;
	std::set<int> epsilon_reachable(const std::set<int> &S) const;
	std::set<int> epsilon_reachable(int i) const;
	bool has_epsilon_transition(int i, int j) const;
};

using NFA_ε = epsilon_non_deterministic_finite_state_automata;
using NFA_epsilon = epsilon_non_deterministic_finite_state_automata;