#ifndef TEST_LIST_HPP
#define TEST_LIST_HPP


#include<list>
#include<iostream>
#include"../list.hpp"
namespace TEST_LIST {

	template<typename L>
	void show_list(const L& list) {
		for (auto &i : list)
			std::cout << i << " ";
		std::cout << "\n";
	}

	void test_list() {
		LightSTL::list<int>list = { 1,2,3,4,5 };
		show_list(list);
		list.insert(list.begin(), 3);
		list.insert(list.begin(), 4);
		show_list(list);
		list.erase(list.begin());
		show_list(list);
	}
}


#endif // !TEST_LIST_HPP
