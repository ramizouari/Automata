#include "timed_automata.h"
#include <chrono>
#include <algorithm>


timed_automata::timed_automata(clock_manager& N, int n, int q0,const std::set<int> &F) 
	:H(N), states(n), transitions(n),first_state(q0),accepting_states(F),invariants(n,always_true)
{
}

void timed_automata::add_transition(int i, int j, const time_predicate & P, std::set<int> clears)
{
	transitions[i][j] = {P,clears};
}

bool timed_automata::execute()
{
	int state = first_state;
	H.reset();
	default_clock::time_point t1 = default_clock::now(), t2 = default_clock::now();
	while (invariants[state](H))
	{
		for (const auto& t : transitions[state])
		{
			if (t.second.first(H))
			{
				state = t.first;
				t1 = default_clock::now();
				H.reset(t.second.second);
				break;
			}
		}
		t2 = default_clock::now();
	}
	return !std::binary_search(accepting_states.begin(), accepting_states.end(), state);
}

bool timed_automata::execute(std::chrono::milliseconds max_blocking_time)
{
	int state=first_state;
	H.reset();
	default_clock::time_point t1=default_clock::now(), t2=default_clock::now();
	while (((t2-t1)<max_blocking_time) && invariants[state](H))
	{
		for (const auto& t : transitions[state])
		{
			if (t.second.first(H))
			{
				state = t.first;
				t1 = default_clock::now();
				H.reset(t.second.second);
				break;
			}
		}
		t2 = default_clock::now();
	}
	return !std::binary_search(accepting_states.begin(), accepting_states.end(), state);
}

bool timed_automata::execute(std::chrono::milliseconds max_blocking_time, int max_iter)
{
	int state = first_state;
	H.reset();
	default_clock::time_point t1 = default_clock::now(), t2 = default_clock::now();

	int k = 1;

	while ((k<max_iter)&&((t2 - t1) < max_blocking_time) && invariants[state](H))
	{
		for (const auto& t : transitions[state])
		{
			if (t.second.first(H))
			{
				state = t.first;
				t1 = default_clock::now();
				H.reset(t.second.second);
				break;
			}
		}
		t2 = default_clock::now();
	}
	return !std::binary_search(accepting_states.begin(), accepting_states.end(), state);
}


void timed_automata::set_invariant(int i, const time_predicate& f)
{
	invariants[i] = f;
}

bool timed_automata::debug(std::chrono::milliseconds max_blocking_time, std::ostream& S)
{
	int state = first_state;
	H.reset();
	default_clock::time_point t1 = default_clock::now(), t2 = default_clock::now();
	S << state << ", ";
	for (auto& t : H.snapshot())
		S << std::chrono::duration_cast<std::chrono::milliseconds>(t).count() << ", ";
	S << "\tdt: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << '\n';
	while (((t2 - t1) < max_blocking_time)&& invariants[state](H))
	{
		for (const auto& t : transitions[state])
		{
			if (t.second.first(H))
			{
				state = t.first;
				H.reset(t.second.second);
				S << state << ", ";
				for (auto& t : H.snapshot())
					S << std::chrono::duration_cast<std::chrono::milliseconds>(t).count() << ", ";
				S << "\tdt: " << std::chrono::duration_cast<std::chrono::milliseconds>(default_clock::now() - t1).count() << '\n';
				t1 = default_clock::now();
				break;
			}
		}
		t2 = default_clock::now();
	}
	return !std::binary_search(accepting_states.begin(), accepting_states.end(), state);
}

bool timed_automata::debug(std::chrono::milliseconds max_blocking_time,int max_iter, std::ostream& S)
{
	int state = first_state;
	H.reset();
	default_clock::time_point t1 = default_clock::now(), t2 = default_clock::now();
	S << state << ", ";
	for (auto& t : H.snapshot())
		S << std::chrono::duration_cast<std::chrono::milliseconds>(t).count() << ", ";
	S << "\tdt: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << '\n';
	int k = 0;
	while (((t2 - t1) < max_blocking_time)&&(k<max_iter) && invariants[state](H))
	{
		for (const auto& t : transitions[state])
		{
			if (t.second.first(H))
			{
				state = t.first;
				k++;
				H.reset(t.second.second);
				S << state << ", ";
				for (auto& t : H.snapshot())
					S << std::chrono::duration_cast<std::chrono::milliseconds>(t).count() << ", ";
				S << "\tdt: " << std::chrono::duration_cast<std::chrono::milliseconds>(default_clock::now() - t1).count() << '\n';
				t1 = default_clock::now();
				break;

			}
		}
		t2 = default_clock::now();
	}
	S << state << ", ";
	for (auto& t : H.snapshot())
		S << std::chrono::duration_cast<std::chrono::milliseconds>(t).count() << ", ";
	S << "\tdt: " << std::chrono::duration_cast<std::chrono::milliseconds>(default_clock::now() - t1).count() << '\n';
	return !std::binary_search(accepting_states.begin(), accepting_states.end(), state);
}

bool always_true(const clock_manager&)
{
	return true;
}
