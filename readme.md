#Automata Library

This library is intended simulate an automata, in particular a Finite State Machine. 
It executes that machine on a string (list of symbols), the result of the execution is whether the automata accepts or rejects that string


Using this construction, it gives the possibility to construct regular expressions on some locales and match them against strings

##1. Automata
An automata is A=(S,Σ,Ω) where Ω is a subset of Σ* representing the language of the automata.
Given a string s in Σ, we say that A accepts s if and only if s in Ω

##2. Finite State Automata
a FSA is defined by a quintuple M = (S,Σ,q0,F,E) where: 
- S is the set of states
- Σ is the set of input symbols
- q0 in S is the first state
- F included in S is the set of accepting states, in other words final states that are accepted by M
- E included in S×Σ×S is the set of transitions(edges) where each edge is (u,e,v): directed from u to v and labeled by e  
- Ω is the set of strings that can be reached from q0

##2.1 Non-Deterministic Finite State Automata
 a NFA is a simply a general FSA M = (S,Σ,q0,F,E)
- Let A(v,e) is the set of vertices who are the endpoint of an edge with label e starting from v

- The execution of a string s= s(0)..s(n-1) in Σ* is as follow
1. if s = ε, then M accepts s iff q0 is a final state
2. if s != ε, then s = s(0)..s(n-1) where s(i) in Σ for every i in 0..n-1, M accepts s iff any machine M' in W 
accepts s' where:
2.1. s'=s(1)..s(n-1)
2.2. W = { (S,Σ,v,F,E), v in A(q0,s(0)) }

This can be informally understood as M accepts s iff there is an execution where M accepts s

##2.2 Non-Deterministic Finite State Automata with ε Transitions: NFA_ε
an NFA_ε is a generalisation of NFA M = (S,Σ,q0,F,E) with the possibility of allowing ε-transitions
- ε-transitions are transitions that do not consume a symbol