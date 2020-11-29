﻿#include "regular_expression.h"
#include <stack>
#include <queue>
#include <iterator>
#include <sstream>

/*
* This helper function is attempted to extract the multiplicity of an element in a regex
* The argument of this function is the next iterator of the iterator who points to '{'
*/
std::pair<int, int> regex::multiplicity(std::string::iterator& a)
{
	std::string S[2];
	int k = 0;
	while (*a != '}')
	{
		if (*a == ',')
			k++;
		else S[k].push_back(*a);
		++a;
	}
	if (k == 0)
		S[1] = S[0];
	std::stringstream stream;
	stream << S[0] << ' ' << S[1];
	int n, m;
	if (stream >> n >> m);
	else m = -1;
	return { n,m };
}

/*
* This helper function is attempted to extract the multiplicity of an element in a regex
* The argument of this function is the next iterator of the iterator who points to '{'
*/
std::pair<int, int> regex::multiplicity(std::list<char>::iterator a)
{
	std::string S[2];
	int k = 0;
	while (*a != '}')
	{
		if (*a == ',')
			k++;
		else S[k].push_back(*a);
		++a;
	}
	if (k == 0)
		S[1] = S[0];
	std::stringstream stream;
	stream << S[0] << ' ' << S[1];
	int n, m;
	if (stream >> n >> m);
	else m = -1;
	return { n,m };
}


regex::regular_expression_converter::regular_expression_converter(std::string S):
	A(reg_machine(standard_form(S)))
{
	A.inplace_convert();
}

/* 
 * Match a string againt the regular expression
 * As a fact, this function constructs a string of symbol D, execute the automata A on D, and checks whether A
 * accepts D or not
 */
bool regex::regular_expression_converter::match(const std::string& S) const
{
	std::deque<int> D;
	std::copy(S.begin(), S.end(), std::back_inserter(D));
	return !A.run(D);
}

/*
* This helper function converts the regular expression to a form understandable by the reg_machine function
*/
std::string regex::regular_expression_converter::standard_form(std::string S)
{
	/*std::string P;

	for (auto it = S.begin();it!=S.end();++it)
	{
		if (it == S.begin())
		{
			if(*it != '^')
				P += ".*";
			else continue;
		}
		if (*it == '{')
		{
			auto it1 = it-1,it2=it;
			if (*it1 == ')')
			{
				int m = 0;
				do
				{
					if (*it1 == ')')
						m++;
					else if (*it1 == '(')
						m--;
					if (it1 == S.begin())
						break;
					if (m == 0)
						break;
					--it1;
				} while (true);
			}
			if ((it1 != S.begin()) && (*(it1 - 1) == '\\'))
				--it1;
			++it;
			auto u = multiplicity(it);
			for (int i = 0; i < u.first - 1; i++)
				std::copy(it1, it2, std::back_inserter(P));
			if (u.first == 0)
			{
				if (u.second == -1)
					P += '*';
				else
				{
					P += '?';

					for (int i = 0; i < u.second - 1; i++)
					{
						std::copy(it1, it2, std::back_inserter(P));
						P += '?';
					}
				}
			}
			else
			{
				if (u.second == -1)
				{
					std::copy(it1, it2, std::back_inserter(P));
					P += '*';
				}
				else
				{
					for (int i = u.first-1; i < u.second - 1; i++)
					{
						std::copy(it1, it2, std::back_inserter(P));
						P += '?';
					}
				}
			}
			continue;
		}
		P += *it;
	}
	if (P.back() == '$')
		P.pop_back();
	else P += ".*";
	if (P.find('}') == std::string::npos)
		return P;
	else return standard_form(P);
	*/

	std::list<char> L;
	std::copy(S.begin(), S.end(), std::back_inserter(L));
	return standard_form(L);
}


/*
* This helper function converts the regular expression to a form understandable by the reg_machine function
* It converts every element :
* - 'a{n,m}' to 'a' n times and 'a?' m-n times
* - 'a{n,}' to 'a' n times and 'a*'
* - If R does not contain '^' as a first symbol, insert '.*' to the beginning of regular expression
* - If R does not contain '$' as a last symbol, insert '.*' to the end of regular expression
*/
std::string regex::regular_expression_converter::standard_form(std::list<char> S)
{
	bool add_begin = true;
	for (auto it = S.begin(); it != S.end();)
	{
		if (it == S.begin() && add_begin)
		{
			add_begin = false;
			if ((*it != '^') )
			{
				S.insert(it, '.');
				S.insert(it,  '*');
				++it;
				continue;
			}
			else
			{
				it = S.erase(it);
				continue;
			}
		}
		if (*it == '{')
		{
			auto it1 = std::prev(it);
			if (*it1 == ')')
			{
				int m = 0;
				do
				{
					if (*it1 == ')')
						m++;
					else if (*it1 == '(')
						m--;
					if (it1 == S.begin())
						break;
					if (m == 0)
						break;
					--it1;
				} while (true);
			}
			if ((it1 != S.begin()) && (*std::prev(it1) == '\\'))
				--it1;
			++it;
			auto u = multiplicity(it);
			--it;
			while (*it != '}')
				it = S.erase(it);
			it=S.erase(it);

			auto it2 = std::prev(it);
			for (int i = 0; i < u.first - 1; i++)
			{
				std::copy(it1, it2, std::inserter(S, it));
				S.insert(it, *it2);
			}
			if (u.first == 0)
			{
				if (u.second == -1)
					S.insert(it,'*');
				else
				{
					S.insert(it,'?');

					for (int i = 0; i < u.second - 1; i++)
					{
						std::copy(it1, it2, std::inserter(S,it));
						S.insert(it, *(it2));
						S.insert(it,'?');
					}
				}
			}
			else
			{
				if (u.second == -1)
				{
					std::copy(it1, it2, std::inserter(S,it));
					S.insert(it, *(it2));
					S.insert(it,'*');
				}
				else
				{
					for (int i = u.first - 1; i < u.second - 1; i++)
					{
						std::copy(it1, it2, std::inserter(S,it));
						S.insert(it, *(it2));
						S.insert(it, '?');
					}
				}
			}
			continue;
		}
		++it;
	}
	if (!S.empty() && S.back() == '$')
		S.pop_back();
	else
	{
		S.push_back('.');
		S.push_back('*');
	}
	std::string standard_S;
	std::copy(S.begin(), S.end(), std::back_inserter(standard_S));
	return standard_S;
}

void append_machine(NFA_ε& M,NFA_ε& B)
{
	int n = M.get_states_number(), m = B.get_states_number();
	M.set_states_number(n + m);
	for (int i = 0; i < m; i++)
		for (auto& E : B.get_transition(i))
			for (auto& e : E.second)
				M.add_transition(n + i, n + E.first, e);
	M.add_transition(n - 1, n);
	M.add_transition(n, n - 1);
	std::set<int> final_set;
	for (auto s : B.get_accepting_states())
		final_set.insert(n+s);
	M.set_accepting_states(final_set);
}

void add_disjunction(NFA_ε& M, NFA_ε& B)
{
	int n = M.get_states_number(), m = B.get_states_number();
	M.set_states_number(n + m);
	for (int i = 0; i < m; i++)
		for (auto& E : B.get_transition(i))
			for (auto& e : E.second)
				M.add_transition(n + i, n + E.first, e);
	M.add_transition(0, n);
	M.add_transition(n, 0);
	std::set<int> B_accepting_set,A_accepting_set= M.get_accepting_states();
	std::set<int> accepting_set;
	std::vector<int> F;
	for (auto s : B.get_accepting_states())
		B_accepting_set.insert(n + s);
	std::set_intersection(A_accepting_set.begin(), A_accepting_set.end(),
		B_accepting_set.begin(), B_accepting_set.end(), std::back_inserter(F));
	for (auto& f : F)
		accepting_set.insert(f);
	M.set_accepting_states(accepting_set);
}

void join_machine(NFA_ε& M, NFA_ε& B,int s)
{
	int n = M.get_states_number(), m = B.get_states_number();
	M.set_states_number(n + m);
	for (int i = 0; i < m; i++)
		for (auto& E : B.get_transition(i))
			for (auto& e : E.second)
				M.add_transition(n + i, n + E.first, e);
	M.add_transition(s, n);
	M.add_transition(n, s);
}


/*
	This function creates a regular expression machine from a given string
*/
regular_expression_automata regex::reg_machine(std::string::iterator a, std::string::iterator b)
{

	/*
	* This function accepts a regular expression R and treats it as '^R$'
	* If you want to create a regular expression machine that is understood as 'find an occurence of R in a string S'
	* then use '.*R.*'
	* 
	* This function has two modes: a recursive and an iterative one
	* 1. The recursive mode is activated when a '(' is read, it finds the correspending ')' and creates a regular expression
	* submachine for the string between '(' and ')', it then join that submachine with the parent machine and deactivate the recursive mode
	* 2. The iterative mode is activated iff the recursive mode is not activated, it reads iteratively a symbol, then if it is a nonspecial one
	* it adds that symbol to the machine. For special symbols:
	* '|': the machine adds a disjunction
	* '*': the machine gives the last symbol/submachine an accepted multiplicity greater or equal then 0
	* '+': the machine gives the last symbol/submachine an accepted multiplicity greater then 0
	* '?': the machine gives the last symbol/submachine an accepdted multiplicity of 0 or 1 
	* 
	* Notes:
	* It is guaranteed that the resulting machine M has only one final state
	*/

	//m is used to delimit the substring in recursive mode
	int m = 0;
	bool recursive_mode = false;
	//final_states is a container of all possible final states if the machine
	std::deque<int> final_states = { 0 };
	regular_expression_automata A(1, 0, { 0 });
	for (auto it = a,it1=a,it2=a; it != b; ++it)
	{
		if (*it == '(')
		{
			if (!recursive_mode)
				it1 = it + 1;
			//activates recursive mode
			recursive_mode = true;
			m++;
		}
		else if (*it == ')')
		{
			m--;

			//the correspending ')' is found
			if (m == 0)
			{
				it2 = it;
				//construct a submachine for the given substring between '(' and ')'
				regular_expression_automata S = reg_machine(it1,it2);
				//by construction, it is guaranteed that S has only one final state
				auto s = *S.get_accepting_states().begin();
				//Configuring multiplicity of the submachine S
				if (it + 1 != b) switch (*(it + 1))
				{
				case '*':
					S.add_transition(0, s);
					S.add_transition(s, 0);
					++it;
					break;
				case '+':
					S.add_transition(s, 0);
					++it;
					break;
				case '?':
					S.add_transition(0, s);
					++it;
					break;
				}
				//gets the last final state of the machine A
				int v = final_states.front();
				final_states.pop_front();
				//replace it with a new final state of A+B
				final_states.push_front(s + A.get_states_number());
				//A <-- A+B 
				join_machine(A, S, v);
				recursive_mode = false;
			}
		}
		else if (!recursive_mode) 
		{
			if (*it == '|')
			{
				//adds epsilon as a final state
				final_states.push_front(0);
				continue;
			}
			
			//gets the last added final state
			int v1 = final_states.front();
			final_states.pop_front();
			
			//Adding a state
			auto v2=A.add_state();
			char c;
			switch (*it)
			{
			case '.':
				c = character_class::any;
				break;
			case '\\':
			{	
				bool skip = true;
				++it;
				switch (*it)
				{
				case 'd':
					c = character_class::numeric;
					break;
				case 'w':
					c = character_class::word;
					break;
				default:
					skip = false;
				}
				if (skip)
					break;
			}
			default:
				c = *it;
			}
			//Configuring multiplicity
			if((it+1)!=b)
			switch (*(it+1))
			{
			case '+':
				A.add_transition(v1, v2, c);
				A.add_transition(v2, v2, c);
				++it;
				break;
			case '*':
				A.add_transition(v1,v2);
				A.add_transition(v2, v2, c);
				++it;
				break;
			case '?':
				A.add_transition(v1, v2);
				A.add_transition(v1, v2, c);
				++it;
				break;
			default:
				A.add_transition(v1, v2, c);
			}
			else A.add_transition(v1, v2, c);

			//replacing final state
			final_states.push_front(v2);
		}
	}

	//Joining all possible final states to a unique equivalent one
	int f;
	if (final_states.size() == 1)
		f = final_states.front();
	else
	{
		f = A.add_state();
		for (auto s : final_states)
			A.add_transition(s, f);
	}
	A.set_accepting_states({ f });


	return A;
}

regular_expression_automata regex::reg_machine(std::string S)
{
	return reg_machine(S.begin(), S.end());
}