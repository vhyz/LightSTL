#ifndef TEST_VECTOR_HPP
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


	void test_vector() {
		LightSTL::vector<int>LightSTL_vec;
		std::vector<int>std_vec;
		//test_vector_template(LightSTL_vec);
		test_vector_template(std_vec);
	}
}


#endif // !TEST_VECTOR_HPP
