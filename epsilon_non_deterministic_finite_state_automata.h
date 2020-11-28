#pragma once
#include "non_deterministic_finite_state_automata.h"
class epsilon_non_deterministic_finite_state_automata : public non_deterministic_finite_state_automata
{
	public:
	using non_deterministic_finite_state_automata::non_deterministic_finite_state_automata;
	void add_transition(int i, int j, int c) override;
	void add_transition(int i, int j);
	static constexpr int ε = -1;
	bool execute(std::deque<int>& S) const override;
	void inplace_convert();
private:
	non_deterministic_finite_state_automata equivalent_NFA() const;
	epsilon_non_deterministic_finite_state_automata reverse() const;
	std::set<int> acceptable_states() const;
	std::set<int> epsilon_reachable(const std::set<int> &S) const;
	std::set<int> epsilon_reachable(int i) const;
	bool has_epsilon_transition(int i, int j) const;
protected:
	epsilon_non_deterministic_finite_state_automata(non_deterministic_finite_state_automata&& A);


};

using NFA_ε = epsilon_non_deterministic_finite_state_automata;
using NFA_epsilon = epsilon_non_deterministic_finite_state_automata;