#pragma once
#include "automata.h"
class repeater :
    public automata
{
    automata* A;
    int min_occ, max_occ;
public:
    repeater(automata* A, int n, int m);
    bool execute(std::deque<int>& S) const override;
};

