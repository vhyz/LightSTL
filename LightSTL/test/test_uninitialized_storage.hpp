#ifndef TEST_UNINITIALIZED_STORAGE_HPP
#define TEST_UNINITIALIZED_STORAGE_HPP
#include <iostream>
#include"../memory/uninitialized_storage.hpp"
#include"../memory/allocator.hpp"
#include<cassert>

namespace test {
	class Bar {
	public:
		Bar(int x):n(x) {
		}
		~Bar() {
		}
		Bar(const Bar& other) {
			n = other.n;
		}
		bool operator==(const Bar& other) {
			return n == other.n;
		}
		bool operator!=(const Bar& other) {
			return n != other.n;
		}
		int n;
	};
	void test_uninitialized_copy() {
		const size_t cap = 100;
		LightSTL::allocator<Bar> alloc;
		Bar* p = alloc.allocate(cap);
		test::Bar bar(0);
		Bar*begin = p;
		Bar*end = p + cap;
		for (size_t i = 0; i < cap; ++i) {
			bar.n = i;
			alloc.construct(p++, bar);
		}
		p = alloc.allocate(cap);
		Bar* begin2 = p;
		Bar* end2 = p + cap;
		LightSTL::uninitialized_copy(begin, end, p);
		Bar* lhs = begin, *rhs = begin2;
		for (size_t i = 0; i < cap; ++i)
			assert(*lhs++ == *rhs++);
		alloc.deallocate(begin,1);
		alloc.deallocate(begin2,1);
	}
	void test_uninitialized_storage() {
		std::cout << "test_uninitialized_storage.hpp\n";
		test_uninitialized_copy();
	}
}





#endif // !TEST_UNINITIALIZED_STORAGE_HPP
