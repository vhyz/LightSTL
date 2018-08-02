#ifndef LIST_HPP

#include<cstddef>
#include"memory/allocator.hpp"
#include"iterator/iterator.hpp"


namespace detail {
	
}

template<class T,class Allocator=LightSTL::allocator<T>>
class list {
private:
	template<class T>
	class list_node {
		list_node* prev;
		list_node* next;
		T data;
	};

public:
	//typedefs
	using value_type = T;
	using allocator_type = Allocator;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = T & ;
	using const_reference = const T&;
	using pointer = Allocator::pointer;
	using const_pointer = Allocator::const_pointer;
	using iterator = T;
	using const_iterator = T;
	using reverse_iterator = LightSTL::reverse_iterator<iterator>;
	using const_reverse_iterator = LightSTL::reverse_iterator<const_iterator>;


};

#endif // !LIST_HPP
