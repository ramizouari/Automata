#pragma once
#include <vector>
#include <set>
#include <map>
#include <string>
#include <deque>

class finite_state_automata
{
protected:
	int states;
	std::vector<std::map<int, std::set<int>>> transitions;
	std::set<int> accepting_states;
	int first_state;
public:
	finite_state_automata(int n, int q0 = 0, const std::set<int>& F = {});
	virtual void add_transition(int i, int j, int c);
	void add_transition(int i, int j);
	void remove_transition(int i, int j, int c);
	std::map<int, std::set<int>> get_transition(int i) const;
	virtual bool run(std::deque<int> sequence) const = 0;
};