#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include<cstddef>
#include"iterator_traits.hpp"
#include"../memory/addressof.hpp"

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


template< class InputIt, class Distance >
constexpr void advance(InputIt& it, Distance n) {
	using category = typename iterator_traits<InputIt>::iterator_category;
}


template< class Iterator >
class reverse_iterator{
public:

	using iterator_type = Iterator;
	using value_type = typename LightSTL::iterator_traits<Iterator>::value_type;
	using difference_type = typename LightSTL::iterator_traits<Iterator>::difference_type;
	using pointer = typename LightSTL::iterator_traits<Iterator>::pointer;
	using reference = typename LightSTL::iterator_traits<Iterator>::reference;
	using iterator_category = typename LightSTL::iterator_traits<Iterator>::iterator_category;

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
		Iterator tmp = current;
		return *--tmp;
	}

	constexpr pointer operator->() const {
		LightSTL::addressof(operator*());
	}

	constexpr reverse_iterator& operator++() {
		current--;
		return *this;
	}
	constexpr reverse_iterator& operator--() {
		current++;
		return *this;
	}
	constexpr reverse_iterator operator++(int) {
		reverse_iterator res = *this;
		current--;
		return res;
	}
	constexpr reverse_iterator operator--(int) {
		reverse_iterator res = *this;
		current++;
		return res;
	}
	constexpr reverse_iterator operator+(difference_type n) const {
		reverse_iterator res = *this;
		LightSTL::advance(current, -n);
		return res;
	}
	constexpr reverse_iterator operator-(difference_type n) const {
		reverse_iterator res = *this;
		LightSTL::advance(current, n);
	}
	constexpr reverse_iterator& operator+=(difference_type n) {
		LightSTL::advance(current, -n);
		return *this;
	}
	constexpr reverse_iterator& operator-=(difference_type n) {
		LightSTL::advance(current, n);
		return *this;
	}

protected:
	Iterator current;
};


template< class Iterator1, class Iterator2 >
constexpr auto operator-(const reverse_iterator<Iterator1>& lhs,
	const reverse_iterator<Iterator2>& rhs
	) -> decltype(rhs.base() - lhs.base()) {
	return rhs.base() - lhs.base();
}

template< class Iterator1, class Iterator2 >
constexpr bool operator==(const reverse_iterator<Iterator1>& lhs,
	const reverse_iterator<Iterator2>& rhs) {
	return lhs.base() == rhs.base();
}
template< class Iterator1, class Iterator2 >
constexpr bool operator!=(const reverse_iterator<Iterator1>& lhs,
	const reverse_iterator<Iterator2>& rhs) {
	return lhs.base() != rhs.base();
}
template< class Iterator1, class Iterator2 >
constexpr bool operator<(const reverse_iterator<Iterator1>& lhs,
	const reverse_iterator<Iterator2>& rhs) {
	return lhs.base() < rhs.base();
}
template< class Iterator1, class Iterator2 >
constexpr bool operator<=(const reverse_iterator<Iterator1>& lhs,
	const reverse_iterator<Iterator2>& rhs) {
	return lhs.base() <= rhs.base();
}
template< class Iterator1, class Iterator2 >
constexpr bool operator>(const reverse_iterator<Iterator1>& lhs,
	const reverse_iterator<Iterator2>& rhs) {
	return lhs.base() > rhs.base();
}
template< class Iterator1, class Iterator2 >
constexpr bool operator>=(const reverse_iterator<Iterator1>& lhs,
	const reverse_iterator<Iterator2>& rhs) {
	return lhs.base() >= rhs.base();
}

}
#endif // !ITERATOR_HPP
