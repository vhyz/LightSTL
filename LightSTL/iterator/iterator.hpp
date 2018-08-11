#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include<cstddef>
#include"iterator_traits.hpp"

namespace LightSTL {

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


protected:
	Iterator current;
};


namespace detail {
	template< class InputIt >
	inline typename iterator_traits<InputIt>::difference_type _distance(InputIt first, InputIt last, input_iterator_tag) {
		typename iterator_traits<InputIt>::difference_type n = 0;
		while (first != last)
			first++;
		return n;
	}

	template< class InputIt >
	inline typename iterator_traits<InputIt>::difference_type _distance(InputIt first, InputIt last, random_access_iterator_tag) {
		return last - first;
	}
}

template< class InputIt >
inline typename iterator_traits<InputIt>::difference_type distance(InputIt first, InputIt last) {
	using category = typename iterator_traits<InputIt>::iterator_category;
	return detail::_distance(first, last, category());
}


namespace detail {

}


template< class InputIt, class Distance >
constexpr void advance(InputIt& it, Distance n) {
	using category = typename iterator_traits<InputIt>::iterator_category;
}


}
#endif // !ITERATOR_HPP
