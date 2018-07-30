#include"../memory/addressof.hpp"
#include<iostream>

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
	std::cout << &bar << std::endl;
	std::cout << LightSTL::addressof(bar) << std::endl;
	std::cout << &n << std::endl;
	std::cout << LightSTL::addressof(n) << std::endl;
}