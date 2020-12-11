#include "regular_expression.h"
#include <stack>
#include <queue>
#include <iterator>
#include <sstream>




void configure_multiplicity(std::string::iterator &a,regular_expression_automata &S,int start,int end)
{
	switch (*(a+1))
	{
	case character_class::kleene_star:
		S.add_transition(start, end);
		S.add_transition(end, start);
		++a;
		break;
	case character_class::kleene_plus:
		S.add_transition(end, start);
		++a;
		break;
	case character_class::kleene_quest:
		S.add_transition(start, end);
		++a;
		break;
	}
}

void regex::join_machine(NFA_ε& M, NFA_ε& B, int s)
{
	int n = M.get_states_number(), m = B.get_states_number();
	M.set_states_number(n + m);
	for (int i = 0; i < m; i++)
		for (auto& E : B.get_transitions(i))
			for (auto& e : E.second)
				M.add_transition(n + i, n + E.first, e);
	M.add_transition(s, n);
	M.add_transition(n, s);
}