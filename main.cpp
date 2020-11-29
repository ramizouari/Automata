#include <iostream>
#include "clock_manager.h"
#include "timed_automata.h"
#include <fstream>
#include "automatas.h"
#include "regular_expression.h"
#include <string>
using namespace std;

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
	regular_expression_converter R(R"(^\d{1,}$)");
	std::string S;
	do
	{
		getline(cin,S);
		cout << R.match(S);
	} while (S != "");
	return false;
}