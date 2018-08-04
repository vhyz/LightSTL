﻿#ifndef TEST_VECTOR_HPP
#define TEST_VECTOR_HPP

#include"../vector.hpp"
#include<iostream>
#include<cassert>
#include<ctime>
#include<vector>

namespace test {
	class A {
	public:

		A(int i=0)  {}
		~A() {
			std::cout << "~A" << std::endl;
		}
	};
	template<typename V>
	void test_vector_template(V& vec) {
		const int n = 1000000;
		clock_t start = clock();
		for (int i = 0; i < n; ++i)
			vec.push_back(i);
		for (int i = 0; i < n; ++i)
			(void)vec[i];
		std::cout << clock() - start << std::endl;
	}

	template<class T>
	void show(LightSTL::vector<T>&vec) {
		for (auto &i : vec)
			std::cout << i << " ";
		std::cout << std::endl;
	}

	void test_insert() {
		LightSTL::vector<int>a = { 1,2,3,4 };
		show(a);
		LightSTL::vector<int>b = { -1,-2 };
		a.insert(a.begin() + 1,4);
		show(a);
		a.insert(a.begin() + 2, b.begin(), b.end());
		show(a);
		a.insert(a.begin() + 1, { 1,2,3,4 });
		show(a);
		a.resize(a.size() * 2);
		show(a);

	}

	void test_speed() {
		LightSTL::vector<int>LightSTL_vec;
		std::vector<int>std_vec;
		test_vector_template(LightSTL_vec);
		test_vector_template(std_vec);
	}

	void test_vector() {
		test_insert();
	}


}


#endif // !TEST_VECTOR_HPP
