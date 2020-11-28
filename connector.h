#pragma once
#include <vector>
#include "automata.h"
class connector:public automata
{
private:
	std::vector<automata*> A_list;
public:
	void add(automata* A);
	bool execute(std::deque<int>& S) const override;
};

