#ifndef ALLOCATOR_TRAITS_HPP
#define ALLOCATOR_TRAITS_HPP


//C++17要求allocator中的某些type弃用
template< class Alloc >
struct allocator_traits {
	using allocator_type = Alloc;
	using value_type = Alloc::value_type;
	using pointer = value_type * ;
	using const_pointer = 
};


#endif // !ALLOCATOR_TRAITS_HPP
