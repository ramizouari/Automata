#include "repeater.h"

repeater::repeater(automata* _A, int n, int m):A(_A),min_occ(n),max_occ(m)
{
}

bool repeater::execute(std::deque<int>& S) const
{
	
	for (int i = 0; i < min_occ; i++)
		if (A->execute(S))
			return true;
	return false;
}
