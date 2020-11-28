#include "char_acceptor.h"

char_acceptor::char_acceptor(char C) :accepted_character(C) {}

bool char_acceptor::execute(std::deque<int>& S) const
{
	if (S.empty())
		return true;
	auto w = S.front();
	S.pop_front();
	return !(w == accepted_character);
}
