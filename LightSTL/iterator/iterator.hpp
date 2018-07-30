#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include<cstddef>


namespace LightSTL {

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};




template<
	class Category,
	class T,
	class Distance = std::ptrdiff_t,
	class Pointer = T * ,
	class Reference = T &
> struct iterator {
	using iterator_category = Category;
	using value_type = T;
	using difference_type = Distance;
	using pointer = Pointer;
	using reference = Reference;
};

template< class Iterator >
class reverse_iterator : public iterator<input_iterator_tag,Iterator> {
public:

	using iterator_type	= Iterator;

	constexpr reverse_iterator() {}

	constexpr explicit reverse_iterator(Iterator x) {
		current = x;
	}

	template< class U >
	constexpr reverse_iterator(const reverse_iterator<U>& other) {
		current = other.base();
	}

	template< class U >
	constexpr reverse_iterator& operator=(const reverse_iterator<U>& other) {
		current = other.base();
	}

	Iterator base() const {
		return current;
	}

	reference operator*() const {
		return *current;
	}

	pointer operator->() const {
		return current;
	}
protected:
	Iterator current;
};


}
#endif // !ITERATOR_HPP
