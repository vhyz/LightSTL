#ifndef TEST_LIST_HPP
#define TEST_LIST_HPP


#include<list>
#include<iostream>

namespace TEST_LIST {


	void test_list() {
		std::list<int>std_list = { 1,2,3,4,5 };
		auto it = std_list.end();
		it--;
		std::cout << *it;
	}
}


#endif // !TEST_LIST_HPP
