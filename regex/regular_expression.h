#pragma once
#include "regular_expression_automata.h"
#include <list>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iterator>


namespace regex
{


	void join_machine(NFA_ε& M, NFA_ε& B, int s);
		/*
	* This function returns the intervals used to describe a class
	*/
	template<typename character>
	std::vector<std::pair<int, int>> classes(typename std::basic_string<character>::iterator& a)
	{
		std::vector<std::pair<int, int>> C;
		if (*a == character_class::left_class)
			++a;
		int u, v;
		while (*a != character_class::right_class)
		{

			u = *a;
			++a;
			if ((*a == character_class::class_dash) && (*(a + 1) != character_class::right_class))
			{
				++a;
				v = *a;
				++a;
			}
			else v = u;
			C.push_back({ u,v });
		}
		return C;
	}

	template<typename character>
	regular_expression_automata reg_machine(typename std::basic_string<character>::iterator a, 
		typename std::basic_string<character>::iterator b,const std::locale &L=std::locale())
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
		regular_expression_automata A(L,1, 0, { 0 });
		for (auto it = a, it1 = a, it2 = a; it != b; ++it)
		{
			//If the carrent value represents a starting of a group
			if (*it == character_class::left_group)
			{
				if (!recursive_mode)
					it1 = it + 1;
				//activates recursive mode
				recursive_mode = true;
				m++;
			}
			//Else if it reprensents the ending of a group
			else if (*it == character_class::right_group)
			{
				m--;

				//the correspending ')' is found
				if (m == 0)
				{
					it2 = it;
					//construct a submachine for the given substring between '(' and ')'
					regular_expression_automata S = reg_machine<character>(it1, it2, L);
					//by construction, it is guaranteed that S has only one final state
					auto s = *S.get_accepting_states().begin();
					//Configuring multiplicity of the submachine S
					if (it + 1 != b) switch (*(it + 1))
					{
					case character_class::kleene_star:
						S.add_transition(0, s);
						S.add_transition(s, 0);
						++it;
						break;
					case character_class::kleene_plus:
						S.add_transition(s, 0);
						++it;
						break;
					case character_class::kleene_quest:
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
			// If it represents a class of characters
			else if (!recursive_mode && (*it == character_class::left_class))
			{

				auto cl = classes<character>(it);
				if (!cl.empty())
				{
					//gets the last added final state
					int v1 = final_states.front();
					final_states.pop_front();
					//Adding a state
					auto v2 = A.add_state();
					bool increment = false;
					for (auto h : cl) for (int c = h.first; c <= h.second; c++)
					{
						if ((it + 1) != b)switch (*(it + 1))
						{
						case character_class::kleene_plus:
							A.add_transition(v1, v2, c);
							A.add_transition(v2, v2, c);
							increment = true;
							break;
						case character_class::kleene_star:
							A.add_transition(v1, v2);
							A.add_transition(v2, v2, c);
							increment = true;
							break;
						case character_class::kleene_quest:
							A.add_transition(v1, v2);
							A.add_transition(v1, v2, c);
							increment = true;
							break;
						default:
							A.add_transition(v1, v2, c);
						}
						else A.add_transition(v1, v2, c);
					}
					if (increment)
						++it;
					final_states.push_front(v2);
				}
				else if (it+1 != b) switch (*(it+1))
						case character_class::kleene_plus:
						case character_class::kleene_star:
						case character_class::kleene_quest:
							++it;
			}
			else if (!recursive_mode)
			{
				if (*it == character_class::disjunction)
				{
					//adds epsilon as a final state
					final_states.push_front(0);
					continue;
				}

				//gets the last added final state
				int v1 = final_states.front();
				final_states.pop_front();

				//Adding a state
				auto v2 = A.add_state();
				character c;
				c = *it;
				//Configuring multiplicity
				if ((it + 1) != b)
					switch (*(it + 1))
					{
					case character_class::kleene_plus:
						A.add_transition(v1, v2, c);
						A.add_transition(v2, v2, c);
						++it;
						break;
					case character_class::kleene_star:
						A.add_transition(v1, v2);
						A.add_transition(v2, v2, c);
						++it;
						break;
					case character_class::kleene_quest:
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


	template<typename character>
	regular_expression_automata reg_machine(std::basic_string<character> S,const std::locale & L =std::locale())
	{
		return reg_machine<character>(S.begin(), S.end(),L);
	}

	/*
	* This helper function is attempted to extract the multiplicity of an element in a regex
	* The argument of this function is the next iterator of the iterator who points to '{'
	*/
	template<typename character>
	std::pair<int, int> multiplicity(typename std::basic_string<character>::iterator& a)
	{
		//a pair of strings, the first containing the lower limit, the second containing the upper limit
		std::basic_string<character> S[2];
		int k = 0;
		//Fill both strings
		while (*a != character('}'))
		{
			if (*a == character(','))
				k++;
			else S[k].push_back(*a);
			++a;
		}
		//If only one limit is given, then it is both the upper and lower limit
		if (k == 0)
			S[1] = S[0];
		//Convert to a pair of integers
		int n=0, m=0,R;
		for (int i = 0, R = 1; i < S[0].size(); i++, R *= 10)
			n += R * (S[0][i] - '0');
		if (S[1].size() == 0)
			m = -1;
		else for (int i = 0, R = 1; i < S[1].size(); i++, R *= 10)
			m += R * (S[1][i] - '0');
		return { n,m };
	}



	/*
	* This helper function converts the regular expression to a form understandable by the reg_machine function
	*/
	template<typename character>
	void convert(std::list<character>& S)
	{
		auto a = S.begin(), b = S.end();
		bool in_class = false;
		if ((a != b) && (*a == '^'))
			*a = character_class::begin;
		while (a != b)
		{
			if ((*a == '\\') && (std::next(a) != b)) switch (*std::next(a))
			{
			case 'w':
				a = S.erase(a);
				*a = character_class::word;
				break;
			case 'd':
				a = S.erase(a);
				*a = character_class::numeric;
				break;
			default:
				a = S.erase(a);
				break;
			}
			else switch (*a)
			{
			case '*':
				if (!in_class)
					*a = character_class::kleene_star;
				break;
			case '+':
				if (!in_class)
					*a = character_class::kleene_plus;
				break;
			case '?':
				if (!in_class)
					*a = character_class::kleene_quest;
				break;
			case '[':
				in_class = true;
				*a = character_class::left_class;
				break;
			case ']':
				in_class = false;
				*a = character_class::right_class;
				break;
			case '(':
				if (!in_class)
					*a = character_class::left_group;
				break;
			case ')':
				if (!in_class)
					*a = character_class::right_group;
				break;
			case '.':
				*a = character_class::any;
				break;
			case '-':
				if (in_class)
					*a = character_class::class_dash;
				break;
			case '|':
				if (in_class)
				{
					a = S.erase(a);
					--a;
				}
				else *a = character_class::disjunction;
				break;
			case '$':
				if (std::next(a) == b)
					*a = character_class::end;
			}
			++a;
		}
	}


	template<typename character>
	std::pair<int, int> multiplicity(typename std::list<character>::iterator a)
	{
		std::basic_string<character> S[2];
		int k = 0;
		while (*a != character('}'))
		{
			if (*a == character(','))
				k++;
			else S[k].push_back(*a);
			++a;
		}
		if (k == 0)
			S[1] = S[0];
		int n=0, m=0;
		for (int i = 0, R = 1,s=S[0].size(); i < s; i++, R *= 10)
			n += R * (S[0][s-i-1] - '0');
		if (S[1].size() == 0)
			m = -1;
		else for (int i = 0, R = 1, s = S[1].size(); i < s; i++, R *= 10)
			m += R * (S[1][s-i-1] - '0');
		return { n,m };
	}


	/*
	* This helper function converts the regular expression to a form understandable by the reg_machine function
	* It converts every element :
	* - 'a{n,m}' to 'a' n times and 'a?' m-n times
	* - 'a{n,}' to 'a' n times and 'a*'
	* - If R does not contain '^' as a first symbol, insert '.*' to the beginning of regular expression
	* - If R does not contain '$' as a last symbol, insert '.*' to the end of regular expression
	*/

	template<typename character>
	std::basic_string<character> standard_form_list( std::list<character> S)
	{
		convert(S);
		bool add_begin = true;
		for (auto it = S.begin(); it != S.end();)
		{
			if (it == S.begin() && add_begin)
			{
				add_begin = false;
				if (*it != character_class::begin)
				{
					S.insert(it, character_class::any);
					S.insert(it, character_class::kleene_star);
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
				if (*it1 == character_class::right_group)
				{
					int m = 0;
					do
					{
						if (*it1 == character_class::right_group)
							m++;
						else if (*it1 == character_class::left_group)
							m--;
						if (it1 == S.begin())
							break;
						if (m == 0)
							break;
						--it1;
					} while (true);
				}
				else if (*it1 == character_class::right_class)
				{
					do
					{
						if (*it1 == character_class::left_class)
							break;
						--it1;
					} while (it1 != S.begin());
				}
				++it;
				auto u = multiplicity<character>(it);
				--it;
				while (*it != '}')
					it = S.erase(it);
				it = S.erase(it);

				auto it2 = std::prev(it);
				for (int i = 0; i < u.first - 1; i++)
				{
					std::copy(it1, it2, std::inserter(S, it));
					S.insert(it, *it2);
				}
				if (u.first == 0)
				{
					if (u.second == -1)
						S.insert(it, character_class::kleene_star);
					else
					{
						S.insert(it, character_class::kleene_quest);

						for (int i = 0; i < u.second - 1; i++)
						{
							std::copy(it1, it2, std::inserter(S, it));
							S.insert(it, *(it2));
							S.insert(it, character_class::kleene_quest);
						}
					}
				}
				else
				{
					if (u.second == -1)
					{
						std::copy(it1, it2, std::inserter(S, it));
						S.insert(it, *(it2));
						S.insert(it, character_class::kleene_star);
					}
					else
					{
						for (int i = u.first - 1; i < u.second - 1; i++)
						{
							std::copy(it1, it2, std::inserter(S, it));
							S.insert(it, *(it2));
							S.insert(it, character_class::kleene_quest);
						}
					}
				}
				continue;
			}
			++it;
		}
		if (!S.empty() && S.back() == character_class::end)
			S.pop_back();
		else
		{

			S.push_back(character_class::any);
			S.push_back(character_class::kleene_star);
		}
		std::basic_string<character> standard_S;
		std::copy(S.begin(), S.end(), std::back_inserter(standard_S));
		return standard_S;
	}

	/*
	* This helper function converts the regular expression to a form understandable by the reg_machine function
	* It converts every element :
	* - 'a{n,m}' to 'a' n times and 'a?' m-n times
	* - 'a{n,}' to 'a' n times and 'a*'
	* - If R does not contain '^' as a first symbol, insert '.*' to the beginning of regular expression
	* - If R does not contain '$' as a last symbol, insert '.*' to the end of regular expression
	*/

	template<typename character>
	std::basic_string<character> standard_form( std::basic_string<character> S)
	{
		std::list<character> L;
		std::copy(S.begin(), S.end(), std::back_inserter(L));
		return standard_form_list<character>(L);
	}





/*
	* This class is responsable for creating a regular expression from a string and locale
	*/
	template<typename character>
	class regular_expression_converter
	{
		regular_expression_automata A;
	public:
		/*
		 This function creates a regular expression machine from a given string
	 */
		regular_expression_converter(std::basic_string< character> S,const std::locale &L=std::locale()): A(reg_machine<character>(standard_form<character>(S),L))
		{
			A.inplace_convert();
		};

		/*
		* Match a string againt the regular expression
		* As a fact, this function constructs a string of symbols D, execute the automata A on D, and checks whether A
		* accepts D or not
		*/
		bool match(const std::basic_string< character>&S) const 
		{
			std::deque<int> D;
			std::copy(S.begin(), S.end(), std::back_inserter(D));
			return !A.run(D);
		}
		;
	private:
	};
}