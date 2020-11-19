#include "deterministic_finite_state_automata.h"

bool deterministic_finite_state_automata::run(std::deque<int> S) const
{
	int state = first_state;
	while (!S.empty())
	{
		auto s = S.front();
		S.pop_front();
		bool stuck = true;
		for (const auto& T : transitions[state])
		{
			if (T.second.contains(s))
			{
				state = T.first;
			 	stuck = false;
			}
		}
		if (stuck)
			return true;
	}
	return !accepting_states.contains(state);
}
