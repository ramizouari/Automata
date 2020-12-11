#include "finite_state_automata.h"
#include <queue>

finite_state_automata::finite_state_automata(int n, int q0, const std::set<int>& F):states(n),transitions(n),accepting_states(F),first_state(q0)
{
}

void finite_state_automata::add_transition(int i, int j, int c)
{
	transitions[i][j].insert(c);
}

void finite_state_automata::add_transition(int i, int j)
{
	finite_state_automata::add_transition(i, j, -1);
}

void finite_state_automata::clear_transitions()
{
	transitions.clear();
	transitions = std::vector<std::map<int,std::set<int>>>(states);
}

void finite_state_automata::set_states_number(int n)
{
	states = n;
	transitions.resize(n);
}

void finite_state_automata::set_initial_state(int q)
{
	first_state = q;
}

void finite_state_automata::set_accepting_states(const std::set<int>& F)
{
	accepting_states = F;
}

int finite_state_automata::get_states_number() const
{
	return states;
}

int finite_state_automata::get_initial_state()
{
	return first_state;
}

std::set<int> finite_state_automata::get_accepting_states()
{
	return accepting_states;
}

void finite_state_automata::remove_transition(int i, int j, int c)
{
	transitions[i][j].erase(c);
	if (transitions[i][j].empty())
		transitions[i].erase(j);
}

int finite_state_automata::add_state()
{
	transitions.push_back({});
	states++;
	return states-1;
}

std::map<int, std::set<int>> finite_state_automata::get_transitions(int i) const
{
	return transitions[i];
}

std::vector<std::map<int, std::set<int>>> finite_state_automata::get_transitions() const
{
	return transitions;
}