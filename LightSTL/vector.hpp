#include"iterator.hpp"
#include"memory/allocator.h"



template<class T, class Allocator = LightSTL::allocator<T> >
class vector {
public:
	using value_type = T;
	using allocator_type = Allocator;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = T & ;
	using const_reference = const T&;
	using pointer = T * ;
	using const_pointer = const T*;
	using iterator = T * ;
	using const_iterator = const T*;
	using reverse_iterator = LightSTL::reverse_iterator<iterator>;
	using const_reverse_iterator = LightSTL::reverse_iterator<const_iterator>;

private:
	iterator start;
	iterator finish;
	iterator end_of_storage;

public:
	vector();
	explicit vector(const Allocator& alloc);
	vector(size_type count,const T& value = T(),const Allocator& alloc = Allocator());
	explicit vector(size_type count, const Allocator& alloc = Allocator());
	template< class InputIt >
	vector(InputIt first, InputIt last,
		const Allocator& alloc = Allocator());
	vector(const vector& other);
	vector(const vector& other, const Allocator& alloc);
	vector(vector&& other)noexcept;
	vector(vector&& other, const Allocator& alloc);
	vector(std::initializer_list<T> init,
		const Allocator& alloc = Allocator());

	iterator begin() {
		return start;
	}

	iterator end() {
		return finish;
	}

	size_type size()const {
		return size_type(finish - start);
	}
};