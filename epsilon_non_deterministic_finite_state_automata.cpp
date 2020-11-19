#include "epsilon_non_deterministic_finite_state_automata.h"
#include <queue>

constexpr int ε = -1;

bool epsilon_non_deterministic_finite_state_automata::run(std::deque<int> sequence) const
{
	/*if (sequence.empty())
		return !accepting_states.contains(first_state);
	int state = first_state;
	epsilon_non_deterministic_finite_state_automata possible_automata(*this);
	auto s = sequence.front();
	bool R;
	for (const auto& T : transitions[state])
	{
		if (T.second.contains(s))
		{
			possible_automata.first_state = T.first;
			sequence.pop_front();
			if (!possible_automata.run(sequence))
				return false;
		}
		else if (T.second.contains(s))
		{
			possible_automata.first_state = T.first;
			if (!possible_automata.run(sequence))
				return false;
		}
	}
	return true;*/
	return equivalent_NFA().run(sequence);
}

void epsilon_non_deterministic_finite_state_automata::add_transition(int i, int j, int c)
{
	if (c < -1)
		throw std::exception("An ε-DFA must have positive labels or ε= -1 label");
	finite_state_automata::add_transition(i, j, c);
}

void epsilon_non_deterministic_finite_state_automata::add_transition(int i, int j)
{
	transitions[i][j].insert(ε);
}

non_deterministic_finite_state_automata epsilon_non_deterministic_finite_state_automata::equivalent_NFA() const
{
	 non_deterministic_finite_state_automata equivalent(states, first_state, acceptable_states());
	 for (int i = 0; i < states; i++)
		 for (const auto& T : transitions[i])
			 for (const auto& e : T.second)
				 equivalent.add_transition(i, T.first, e);

	 bool changed;
	 for (int k = 0; k <= states; k++)
	 {
		 changed = false;
		 for (int i = 0; i < states; i++)
		 {
			 for (const auto& T : equivalent.get_transition(i))
			 {
				 if (T.second.contains(ε))
				 {
					 equivalent.remove_transition(i, T.first, ε);
					 changed = true;
					 if (i != T.first) for (const auto& E : transitions[T.first])
					 {
						 for (const auto& e : E.second)
							 equivalent.add_transition(i, E.first, e);
					 }
				 }
			 }
		 }
		 if (!changed)
			 break;
	 }
	 return equivalent;
}

epsilon_non_deterministic_finite_state_automata epsilon_non_deterministic_finite_state_automata::reverse() const
{
	epsilon_non_deterministic_finite_state_automata R(states, first_state, accepting_states);
	for (int i = 0; i < states; i++)
		for(auto &T:transitions[i])
			R.transitions[T.first][i] = this->transitions[i].at(T.first);
	return R;
}

std::set<int> epsilon_non_deterministic_finite_state_automata::acceptable_states() const
{
	auto R = reverse();
	return (R.epsilon_reachable(accepting_states));
}

std::set<int> epsilon_non_deterministic_finite_state_automata::epsilon_reachable(const std::set<int> &S) const
{
	std::vector<bool> reachable(states, false);
	std::queue<int> Q;
	for (const auto& s : S)
	{
		if (reachable[s])
			continue;
		Q.push(s);
		reachable[s] = true;
		while (!Q.empty())
		{
			auto w = Q.front();
			Q.pop();
			for (const auto& T : transitions[w])
			{
				if (reachable[T.first])
					continue;
				if(has_epsilon_transition(w,T.first))
				{
					Q.push(T.first);
					reachable[T.first] = true;
				}
			}
		}
	}
	std::set<int>reachable_set;
	for (int i = 0; i < states; i++)
		if (reachable[i])
			reachable_set.insert(i);
	return reachable_set;
}


std::set<int> epsilon_non_deterministic_finite_state_automata::epsilon_reachable(int i) const
{
	return epsilon_reachable(std::set<int>({ i }));
}

bool epsilon_non_deterministic_finite_state_automata::has_epsilon_transition(int i, int j) const
{
	auto w = transitions[i].find(j);
	if (w==transitions[i].cend())
		return false;
	if (w->second.empty())
		return false;
	return *w->second.begin() == ε;
}
