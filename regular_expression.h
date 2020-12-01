#pragma once
#include "regular_expression_automata.h"
#include <list>

namespace regex
{
	class regular_expression_converter
	{
		regular_expression_automata A;
	public:
		regular_expression_converter(std::string S);
		bool match(const std::string &S) const;
	private:
		std::string standard_form(std::string S);
		std::string standard_form(std::list<char> S);

	};
	regular_expression_automata reg_machine(std::string::iterator a, std::string::iterator b);
	regular_expression_automata reg_machine(std::string S);
	
	template<typename character>
	std::pair<int, int> multiplicity(typename std::basic_string<character>::iterator& a)
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
		std::basic_stringstream<character> stream;
		stream << S[0] << character(' ') << S[1];
		int n, m;
		if (stream >> n >> m);
		else m = -1;
		return { n,m };
	}

	std::pair<int, int> multiplicity(std::list<char>::iterator a);

}