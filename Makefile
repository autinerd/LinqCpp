CXX=clang++
CXXFLAGS=-Wall -pedantic -Werror -std=c++11 -pipe

linux:
	$(CXX) $(CXXFLAGS) -o linqTest LinqCpp/*.cpp

test:
	linqTest > test.txt
	diff test.txt correct.txt