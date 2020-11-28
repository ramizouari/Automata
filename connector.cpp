#include "connector.h"

void connector::add(automata* A)
{
	A_list.push_back(A);
}

bool connector::execute(std::deque<int>& S) const
{
	for (auto A : A_list)
		if (A->execute(S))
			return true;
	return false;
}
