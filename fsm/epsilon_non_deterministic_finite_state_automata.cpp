#include "epsilon_non_deterministic_finite_state_automata.h"
#include <queue>



bool epsilon_non_deterministic_finite_state_automata::execute(std::deque<int> &sequence) const
{
	return equivalent_NFA2().run(sequence);
}

void epsilon_non_deterministic_finite_state_automata::inplace_convert()
{
	auto NFA_machine = equivalent_NFA2();
	this->accepting_states = std::move(NFA_machine.get_accepting_states());
	this->first_state = NFA_machine.get_initial_state();
	this->states = NFA_machine.get_states_number();
	this->transitions = std::move(NFA_machine.get_transitions());
}

void epsilon_non_deterministic_finite_state_automata::add_transition(int i, int j, int c)
{
	//if (c < -1)
		//throw std::exception("An ε-DFA must have positive labels or ε= -1 label");
	finite_state_automata::add_transition(i, j, c);
}

void epsilon_non_deterministic_finite_state_automata::add_transition(int i, int j)
{
	transitions[i][j].insert(ε);
}

non_deterministic_finite_state_automata epsilon_non_deterministic_finite_state_automata::equivalent_NFA() const
{
	//An equivalent, but slow code
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
			 for (const auto& T : equivalent.get_transitions(i))
			 {
				 if (T.second.count(ε))
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

non_deterministic_finite_state_automata epsilon_non_deterministic_finite_state_automata::equivalent_NFA2() const
{
	non_deterministic_finite_state_automata equivalent(states, first_state, acceptable_states());
	/*
	* Let M = (S,Σ,q0,F,E) an ε-NFA with: 
	* 1. states set S
	* 2. input symbols set Σ which contains the empty symbol ε
	* 3. first state q0
	* 4. accepting states set F
	* 5. edges set E
	* Definition: let v1 and v2 two vertices, 
	* - v2 is epsilon reachable from v1 iff there is a trajectory from v1 too v2 consisting of epsilon edges only
	* - An ε-edge is an edge whose weight is ε
	* - A unit cost trajectory from v1 is a trajectory whose edges are all ε-edges except the last one
	* 
	* Algorithm:
	* Let N(S,Σ,q,F',E') be the equivalent NFA
	* 1. We will construct E' as follow:
	* For every vertice v in S, For every unit trajectory T from v, we will add in N an edge from v to the other endpoint
	* of T with label p, where p is the same label of the last edge of T
	* 2. Let R the reverse machine of N (The machine with the reverse transitions), For every v in S
	* Let H(v) the set of ε-reachable vertices from v in R, then we have F' is the union of H(v) for v in F 
	* This algorithm will do a breadth-first search with ε-edges
	*/

	//For every vertice i in S
	for(int i=0;i<states;i++)
	{
		//We will store the visited vertices in a queue
		std::queue<int> Q({ i });

		//We will mark here every visited vertice
		std::vector<bool> visited(states);

		//while there is another path
		while (!Q.empty())
		{
			//get the first visited non-processed edge
			auto w = Q.front();
			//pop it from the queue
			Q.pop();

			//For every edge starting from w
			for (const auto& T : transitions[w])
			{
				//if the other endpoint is w itself, then for every edge from w to itself with label e
				if (T.first == w) for (auto& e : T.second)
				{
					/*
					* if e is not an ε, by supposition we have an ε-trajectory L from i to w
					* So there is a unit cost trajectory from i to w with the last edge label e
					* So, add a transition from i to T.first with label e
					*/
					if (e != ε)
						equivalent.add_transition(i, T.first, e);
				}
				//else T.first is different from w
				else 
				{
					//if that vertice is already visited, just skip it
					if (visited[T.first])
						continue;
					//else mark it as visited
					visited[T.first] = true;
					//for every edge with label e from w to T.first
					for (const auto& e : T.second)
					{
						//if e is ε, then T.first is ε-reachable from i, add it to the queue
						if (e == ε)
							Q.push(T.first);
						/*
						* there is a unit cost trajectory from i to T.first, this can be proved by remarking that
						* every added vertice to the queue is ε-reachabile from i
						*/
						else equivalent.add_transition(i, T.first, e);
					}
				}
			}
		}
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
				if(T.second.count(ε))
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

epsilon_non_deterministic_finite_state_automata::epsilon_non_deterministic_finite_state_automata(non_deterministic_finite_state_automata&& A):non_deterministic_finite_state_automata(std::move(A))
{
}
