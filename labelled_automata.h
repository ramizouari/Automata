#pragma once
#include "fsm/finite_state_automata.h"
#include <map>
#include <iterator>
#include <algorithm>

template<typename T1,typename T2>
class automata_labeller :public automata
{
	finite_state_automata& automata;
	std::map<T1, int> state_transform;
	std::map<int, T1> state_inverse_transform;
	std::map<T2, int> transition_transform;
	std::map<int, T2> transition_inverse_transform;
	automata_labeller(finite_state_automata& A) :automata(A) {}
public:
	bool execute(const std::deque<T2> &S) const 
	{
		std::deque<int> S0;
		std::transform(S.begin(), S.end(), std::back_inserter(S0),
			[&transition_transform](auto w) {return transition_transform[w]; });
		return automata.run(S0); 
	};
	void add_transition(T1 a, T1 b, T2 v)
	{
		automata.add_transition(state_transform[a], state_transform[b], transition_transform[v]);
	}
	static automata_labeller build(finite_state_automata& A, int n, T1 q0=0, const std::set<T1>& F = {})
	{
		automata_labeller AL(A);
		std::set<int> F0;
		std::transform(F.begin(), F.end(), std::back_inserter(F0), 
			[&state_transform](auto w) {return state_transform[w]; });
		A.set_accepting_states(F0);
		A.set_initial_state(state_transform[q0]);
		A.set_states_number(n);
		return AL;
	}

	void set_states_label(const std::map<T1, int>& transform)
	{
		state_transform = transform;
	}

	void set_states_label(const std::map<char, int>& transform)
	{
		transition_transform = transform;
	}
};


template<typename T1>
class automata_labeller<T1,char>
{
	finite_state_automata& automata;
	std::map<T1, int> state_transform;
	std::map<int, T1> state_inverse_transform;
	std::map<char, int> transition_transform;
	std::map<int, char> transition_inverse_transform;
	automata_labeller(finite_state_automata& A) :automata(A) {}
public:
	bool run(const std::deque<char>& S) const
	{
		std::deque<int> S0;
		std::transform(S.begin(), S.end(), std::back_inserter(S0),
			[&transition_transform](auto w) {return transition_transform[w]; });
		return automata.run(S0);
	};

	bool run(const std::string& S) const
	{
		std::deque<int> S0;
		std::transform(S.begin(), S.end(), std::back_inserter(S0),
			[&transition_transform](auto w) {return transition_transform[w]; });
		return automata.run(S0);
	};
	void add_transition(T1 a, T1 b, char v)
	{
		automata.add_transition(state_transform[a], state_transform[b], transition_transform[v]);
	}
	static automata_labeller build(finite_state_automata& A, int n, T1 q0 = 0, const std::set<T1>& F = {})
	{
		automata_labeller AL(A);
		std::set<int> F0;
		std::transform(F.begin(), F.end(), std::back_inserter(F0),
			[&state_transform](auto w) {return state_transform[w]; });
		A.set_accepting_states(F0);
		A.set_initial_state(state_transform[q0]);
		A.set_states_number(n);
		return AL;
	}

	void set_states_label(const std::map<T1,int> &transform)
	{
		state_transform = transform;
	}

	void set_transitions_label(const std::map<char, int>& transform)
	{
		transition_transform = transform;
	}
};