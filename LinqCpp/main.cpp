//#include "linq.hpp"
#include <iostream>
#include <string>
#define println(x) std::cout << (x) << std::endl
#include <vector>
#include <iterator>
#include <algorithm>
#include <random>
#include "linq.hpp"
int main()
{


	Queryable<int> test({ 1,2,3,4,5 });
	std::vector<int> v(test.ToVector());
	println(test.Any([](int x) { return x < 3; }));
	println(test.Contains(1));
	println(test.Sum());
	println(test.Where([](int x) { return x < 3; }).Sum());
	println(test.Take(4).Sum());
	println(test.Aggregate([](int agg, int next) -> int {
		return agg + next;
	}));
	println(test.Aggregate<double>(0, [](double agg, int next) -> double {
		return agg + next;
	}));
	std::vector<int> v2({ 1234, 28347, 37, 3477, 987 });
	return 0;
}