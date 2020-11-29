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
	std::pair<int, int> multiplicity(std::string::iterator& a);
	std::pair<int, int> multiplicity(std::list<char>::iterator a);

}

