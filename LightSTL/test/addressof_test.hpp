#include"../memory/addressof.hpp"
#include<iostream>
#include<cassert>


class Bar {
public:
	Bar * operator& () {
		return nullptr;
	}
};

void addressof_test() {
	Bar bar;
	int n = 1;
	std::cout << "addressof_test.hpp\n";
	assert(&bar == nullptr);
	assert(LightSTL::addressof(bar) != &bar);
	assert(&n == LightSTL::addressof(n));
}
