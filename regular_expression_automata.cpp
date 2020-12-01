#include "regular_expression_automata.h"
#include <algorithm>
#include <iterator>

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
				if (T.second.contains(character_class::any))
					S.insert(T.first);
				else if (T.second.contains(w))
					S.insert(T.first);
				else if (isalpha(w) && T.second.contains(character_class::alpha))
					S.insert(T.first);
				else if (isalnum(w) && T.second.contains(character_class::word))
					S.insert(T.first);
				else if (isdigit(w) && T.second.contains(character_class::numeric))
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
	if ((a >= 256) || (b >= 256))
		throw std::exception("A class with ASCII range that surpasses 256 is not Supported Yet");
	else if (a > b)
		throw std::exception("a cannot be greater than b");
	else if (a == b)
		epsilon_non_deterministic_finite_state_automata::add_transition(i, j, a);
	else {
		intervals_number++;
		transitions[i][j].insert(intervals_number);
		intervals[intervals_number] = { a,b };
	}
}

void regular_expression_automata::add_transition(int i, int j, character_class c)
{
	transitions[i][j].insert(static_cast<int>(c));
}
