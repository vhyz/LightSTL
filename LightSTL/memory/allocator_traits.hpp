#ifndef ALLOCATOR_TRAITS_HPP
#define ALLOCATOR_TRAITS_HPP


//C++17要求allocator中的某些type弃用
template< class Alloc >
struct allocator_traits {
	using allocator_type = Alloc;
	using value_type = typename Alloc::value_type;
	using pointer = typename Alloc::pointer;
	using const_pointer = typename Alloc::const_pointer;
	using difference_type = typename Alloc::difference_type; 
};


#endif // !ALLOCATOR_TRAITS_HPP
