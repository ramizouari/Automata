#pragma once
#include <vector>
#include <set>
#include <map>
#include <string>
#include <deque>
#include "automata.h"

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