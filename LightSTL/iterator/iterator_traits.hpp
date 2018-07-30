#ifndef ITERATOR_TRAITS_HPP
#define ITERATOR_TRAITS_HPP



namespace LightSTL {

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

}
#endif // !ITERATOR_TRAITS_HPP
