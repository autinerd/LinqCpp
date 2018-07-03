#include "linq.hpp"
#include <iostream>
#include <string>
#define println(x) std::cout << (x) << std::endl
#include <vector>
#include <iterator>
#include <algorithm>
#include <random>

int checksum(int num)
{
	int summe = 0;
	while (num > 0)
	{
		summe += num % 10;
		num /= 10;
	}
	return summe;
}

template<class It>
void print(It begin, It end)
{
	while (begin != end)
	{
		std::cout << *begin << std::endl;
		begin++;
	}
}
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

	std::sort(v2.begin(), v2.end(), [](int a, int b) {
		return checksum(a) < checksum(b);
	});
	print(v2.begin(), v2.end());
	return 0;
}