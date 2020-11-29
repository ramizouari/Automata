#pragma once
#include "deterministic_finite_state_automata.h"
/*
*  a NFA is a FSA M = (S,Σ,q0,F,E)
* - Let Σ* is the set of strings over Σ
* - Let A(v,e) is the set of vertices who are the endpoint of an edge with label e starting from v
*
* - The execution of a string s= s(0)..s(n-1) in Σ* is as follow
* 1. if s = ε, then M accepts s iff q0 is a final state
* 2. if s != ε, then s = s(0)..s(n-1) where s(i) in Σ for every i in 0..n-1, M accepts s iff any machine M' in W 
* accepts s' where:
* 2.1. s'=s(1)..s(n-1)
* 2.2. W = { (S,Σ,v,F,E), v in A(q0,s(0)) }
* 
* This can be informally understood as M accepts s iff there is an execution where M accepts s
*/
class non_deterministic_finite_state_automata: public finite_state_automata
{
public:
	using finite_state_automata::finite_state_automata;
	bool execute(std::deque<int>& s) const override;
private:
};

using NFA = non_deterministic_finite_state_automata;

