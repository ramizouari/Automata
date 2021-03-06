#include "non_deterministic_finite_state_automata.h"
#include <algorithm>
#include <iterator>

bool non_deterministic_finite_state_automata::execute(std::deque<int>& sequence) const
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
				if (T.second.count(w)) 
					S.insert(T.first);
			}
		}
		possible_states = std::move(S);
		if (possible_states.empty())
			return true;
	}
	std::vector<int> R;
	std::set_intersection(possible_states.begin(), possible_states.end(),
		accepting_states.begin(),accepting_states.end(),
		std::back_inserter(R));
	return R.empty();
}