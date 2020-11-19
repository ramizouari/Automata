#pragma once
#include <chrono>
#include <vector>
#include <set>
#include <functional>

using default_clock = std::chrono::high_resolution_clock;
class clock_manager
{
private:
	std::vector<std::chrono::time_point<default_clock>> T;
public:
	clock_manager(int n);
	void reset();
	void reset(int i);
	void reset(const std::set<int> &S);
	std::vector<default_clock::duration> snapshot() const;
	default_clock::duration operator[](int i) const;
	default_clock::time_point reference_time(int i) const;

};

