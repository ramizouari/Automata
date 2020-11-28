#pragma once
#include "abstract_acceptor.h"
class char_acceptor :public abstract_acceptor
{
private:
	char accepted_character;
public:
	char_acceptor(char C);
	bool execute(std::deque<int>& S) const override;
};