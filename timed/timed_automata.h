#pragma once
#include <map>
#include <vector>
#include <functional>
#include "clock_manager.h"
#include <ostream>


using time_predicate = std::function<bool(const clock_manager&)>;
bool always_true(const clock_manager&);
class timed_automata
{
private:
	int states;
	std::vector<time_predicate> invariants;
	std::vector<std::map<int, std::pair<time_predicate,std::set<int>>>> transitions;
	clock_manager& H;
	std::set<int> accepting_states;
	int first_state;
public:
	timed_automata(clock_manager& N, int n, int q0 = 0,const std::set<int> & F = {});
	void add_transition(int i,int j,const time_predicate & P,std::set<int> clears);
	bool execute(std::chrono::milliseconds max_blocking_time, int max_iter);
	bool execute(std::chrono::milliseconds max_blocking_time);
	bool execute();
	void set_invariant(int i, const time_predicate& f);
	bool debug(std::chrono::milliseconds max_blocking_time,std::ostream &S);
	bool debug(std::chrono::milliseconds max_blocking_time,int max_iter, std::ostream& S);
private:
	bool start(default_clock::time_point& t1, default_clock::time_point& t2,const std::function<bool()> &continuing_predicate);
	bool start_debug(default_clock::time_point& t1, default_clock::time_point& t2,
		const std::function<bool()>& continuing_predicate,std::ostream &S);
};