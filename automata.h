#pragma once
#include <deque>

class automata
{
public:
	bool run(std::deque<int> S) const;
	virtual bool execute(std::deque<int>& S) const = 0;
};