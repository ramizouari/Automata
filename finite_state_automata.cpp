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

void finite_state_automata::remove_transition(int i, int j, int c)
{
	transitions[i][j].erase(c);
	if (transitions[i][j].empty())
		transitions[i].erase(j);
}

std::map<int, std::set<int>> finite_state_automata::get_transition(int i) const
{
	return transitions[i];
}
