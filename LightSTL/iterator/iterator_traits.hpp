#ifndef ITERATOR_TRAITS_HPP
#define ITERATOR_TRAITS_HPP
#include"iterator.hpp"


namespace LightSTL {

template< class Iterator >
struct iterator_traits {
	using difference_type = typename Iterator::difference_type;
	using value_type = typename Iterator::value_type;
	using pointer = typename Iterator::pointer;
	using reference = typename Iterator::reference;
	using iterator_category = typename Iterator::iterator_category;
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

}
#endif // !ITERATOR_TRAITS_HPP
