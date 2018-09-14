#ifndef TEST_ALLOCATOR_HPP
#define TEST_ALLOCATOR_HPP

#include<iostream>
#include"../memory/allocator.hpp"

namespace TEST_allocator {
	class Bar {
	public:
		Bar() {
			std::cout << "default\n";
		}
		Bar(const Bar&) {
			std::cout << "copy\n";
		}
		Bar(Bar&&) {
			std::cout << "move\n";
		}
		~Bar() {
			std::cout << "delete\n";
		}
	};

	void test_allocator() {
		LightSTL::allocator<Bar>alloc;
		Bar bar;
		Bar* p1 = alloc.allocate();
		Bar* p2 = alloc.allocate();
		Bar* p3 = alloc.allocate();
		alloc.construct(p1);
		alloc.construct(p2, bar);
		alloc.construct(p3, std::move(bar));
		alloc.destory(p1);
		alloc.destory(p2);
		alloc.destory(p3);
		alloc.deallocate(p1,1);
		alloc.deallocate(p2,1);
		alloc.deallocate(p3,1);
	}
}

#endif // !TEST_ALLOCATOR_HPP
