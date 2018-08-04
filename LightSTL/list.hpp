#ifndef LIST_HPP

#include<cstddef>
#include"memory/allocator.hpp"
#include"iterator/iterator.hpp"


namespace detail {
	
}

template<class T,class Allocator=LightSTL::allocator<T>>
class list {

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

private:
	//node
	template<class T>
	struct node {
		node* prev;
		node* next;
		T data;

		template<class... Args>
		node(node* p, node* n, Args&&... args)
			: prev(p), next(n), data(std::forward<Args>(args)...) {}
	};

	node* _head;
	node* _back;
	size_type _size;
	Allocator data_alloc;
public:
	//构造函数
	list()
		: _head(nullptr), _back(nullptr), _size(0), data_alloc() {}

	explicit list(const Allocator& alloc)
		: _head(nullptr), _back(nullptr), _size(0), data_alloc(alloc) {}

	list(size_type count, const T& value, const Allocator& alloc = Allocator());
	explicit list(size_type count, const Allocator& alloc = Allocator());
	template< class InputIt >
	list(InputIt first, InputIt last,
		const Allocator& alloc = Allocator());
	list(const list& other);
	list(const list& other, const Allocator& alloc);
	list(list&& other);
	list(list&& other, const Allocator& alloc);
	list(std::initializer_list<T> init, const Allocator& alloc = Allocator());
};

#endif // !LIST_HPP
