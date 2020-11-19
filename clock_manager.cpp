#include "clock_manager.h"

clock_manager::clock_manager(int n):T(n,default_clock::now())
{
}

void clock_manager::reset()
{
	auto current_time = default_clock::now();
	for (auto& t : T)
		t = current_time;
}

void clock_manager::reset(int i)
{
	T[i] = default_clock::now();
}

void clock_manager::reset(const std::set<int> &S)
{
	auto current_time = default_clock::now();
	for (auto& s : S)
		T[s] = current_time;
}

std::vector<default_clock::duration> clock_manager::snapshot() const
{
	auto n = T.size();
	std::vector<default_clock::duration> S(n);
	auto s = default_clock::now();
	for (int i = 0; i < n; i++)
		S[i] = s - T[i];
	return S;
}

default_clock::duration clock_manager::operator[](int i) const
{
	return default_clock::now()-T[i];
}

default_clock::time_point clock_manager::reference_time(int i) const
{
	return T[i];
}
