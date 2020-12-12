#include "regular_expression_automata.h"
#include <algorithm>
#include <iterator>
#include <locale>

#include <algorithm>
#include <iterator>


regular_expression_automata::regular_expression_automata(const std::locale& L,int n, int q0, const std::set<int>& F):epsilon_non_deterministic_finite_state_automata(n,q0,F),loc(L)
{
}

regular_expression_automata::regular_expression_automata(int n, int q0, const std::set<int>& F) : epsilon_non_deterministic_finite_state_automata(n, q0, F),loc(std::locale())
{
}

bool regular_expression_automata::execute(std::deque<int>& sequence) const
{
	std::set<int> possible_states;
	possible_states = { first_state };
	while (!sequence.empty())
	{
		auto w = sequence.front();
		sequence.pop_front();
		std::set<int> S;
		for (const auto& s : possible_states)
		{
			for (const auto& T : transitions[s])
			{

				//To do, replace it with a single condition, done in a lazy style
				if (T.second.count(character_class::any))
					S.insert(T.first);
				else if (T.second.count(w))
					S.insert(T.first);
				else if (std::isalpha<char>(w, loc) && T.second.count(character_class::alpha))
					S.insert(T.first);
				else if (std::isalnum<char>(w, loc) && T.second.count(character_class::word))
					S.insert(T.first);
				else if (std::isdigit<char>(w, loc) && T.second.count(character_class::numeric))
					S.insert(T.first);
			}
		}
		possible_states = std::move(S);
		if (possible_states.empty())
			return true;
	}
	std::vector<int> R;
	std::set_intersection(possible_states.begin(), possible_states.end(),
		accepting_states.begin(), accepting_states.end(),
		std::back_inserter(R));
	return R.empty();
}

void regular_expression_automata::add_transition(int i, int j, int a, int b)
{
	for(int k=a;k<=b;k++)
		transitions[i][j].insert(k);
}

void regular_expression_automata::add_transition(int i, int j, character_class c)
{
	transitions[i][j].insert(static_cast<int>(c));
}