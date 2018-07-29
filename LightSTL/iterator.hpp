#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include<cstddef>


struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};


template< class Iterator >
struct iterator_traits {
	using difference_type = Iterator::difference_type;
	using value_type = Iterator::value_type;
	using pointer = Iterator::pointer;
	using reference = Iterator::reference;
	using iterator_category = Iterator::iterator_category;
};

template< class T >
struct iterator_traits<T*> {
	using difference_type = std::ptrdiff_t;
	using value_type = T;
	using pointer = T * ;
	using reference = T & ;
	using iterator_category = random_access_iterator_tag;
};

template< class T >
struct iterator_traits<const T*> {
	using difference_type = std::ptrdiff_t;
	using value_type = T;
	using pointer = const T *;
	using reference = const T &;
	using iterator_category = random_access_iterator_tag;
};

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
class reverse_iterator : public iterator {
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

	constexpr Iterator base() const {
		return current;
	}

	constexpr reference operator*() const {

	}

	constexpr pointer operator->() const {

	}
protected:
	Iterator current;
};
#endif // !ITERATOR_HPP
