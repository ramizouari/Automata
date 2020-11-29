#pragma once
#include <vector>
#include <set>
#include <map>
#include <string>
#include <deque>
#include "automata.h"

/*
* This class is the class of finite state automata
* a FSA is defined by a quintuple M = (S,Σ,q0,F,E) where: 
* - S is the set of states
* - Σ is the set of input symbols
* - q0 in S is the first state
* - F included in S is the set of accepting states, in other words final states that are accepted by M
* - E included in S×Σ×S is the set of transitions(edges) where each edge is (u,e,v): 
* directed from u to v and labeled by e  

*/

class finite_state_automata:public automata
{
protected:
	int states;
	std::vector<std::map<int, std::set<int>>> transitions;
	std::set<int> accepting_states;
	int first_state;
public:
	finite_state_automata(int n=0, int q0 = 0, const std::set<int>& F = {});
	virtual void add_transition(int i, int j, int c);
	void add_transition(int i, int j);
	void clear_transitions();
	void set_states_number(int n);
	void set_initial_state(int q);
	void set_accepting_states(const std::set<int>& F);
	int get_states_number() const;
	int get_initial_state();
	std::set<int> get_accepting_states();
	void remove_transition(int i, int j, int c);
	int add_state();
	std::map<int, std::set<int>> get_transition(int i) const;
	virtual bool execute(std::deque<int> &sequence) const = 0;
};