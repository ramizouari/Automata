#include <iostream>
#include "clock_manager.h"
#include "timed_automata.h"
#include <fstream>
#include "automatas.h"
#include "regular_expression.h"
#include <string>
#include <fstream>
using namespace std;


/*
* This class is an example of a custom locale
* It gives the ability to recognize arabic digits as digits
*/
struct arabic_digit_ctype : std::ctype<char32_t>
{
	std::u32string S = U"٠١٢٣٤٥٦٧٨٩";

	bool do_is(mask m, char_type c) const {
		if ((m & digit) && std::binary_search(S.begin(), S.end(), c))
			return true; // Arabic digits will be classified as digits
		return ctype::do_is(m, c); // leave the rest to the parent class
	}
};
int main()
{
	int n=6,m = 2;
	/*clock_manager H(m);
	timed_automata A(H, n, 0, { 0,2,4 });
	ofstream file("result.txt");
	A.add_transition(0, 1, [](const clock_manager& H)->bool {return (H[0] > 5ms)&&(H[1] > 35ms); }, { 0,1 });
	A.add_transition(1, 2, [](const clock_manager& H)->bool {return (H[1] > 30ms); }, { 1 });
	A.add_transition(2, 3, [](const clock_manager& H)->bool {return (H[1] > 5ms) && (H[0] > 35ms); }, { 0,1 });
	A.add_transition(3, 0, [](const clock_manager& H)->bool {return (H[0] > 30ms); }, { 0 });
	A.add_transition(0, 4, [](const clock_manager& H)->bool {return (H[0] > 5ms); }, { 0 });
	A.add_transition(2, 4, [](const clock_manager& H)->bool {return (H[1] > 5ms); }, { 1 });
	A.add_transition(4, 1, [](const clock_manager& H)->bool {return (H[1] > 5ms); }, { 1 });
	A.add_transition(4, 3, [](const clock_manager& H)->bool {return (H[0] > 5ms); }, { 0 });
	A.debug(200ms,1000,file);*/

	using regex::regular_expression_converter;

	std::locale loc(std::locale(""), new arabic_digit_ctype);

	std::locale::global(std::locale(""));

	regular_expression_converter<char32_t> R(UR"(^\d$)",loc);
	std::u32string S;
	std::basic_ifstream<char32_t> file("input.txt");
	while(getline(file, S))
	{
		cout << R.match(S);
	}
	return false;
}