﻿#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

#include<cstddef>  //std::size_t , std::ptrdiff_t
#include<new>
#include"addressof.hpp"



namespace LightSTL{

template<typename T>
class allocator {
public:
	using value_type = T;
	using pointer = T* ;
	using const_pointer = const T*;
	using reference = T & ;
	using const_reference = const T&;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	template<typename U>struct rebind { using other = allocator<U>; };

	allocator()noexcept {} ;
	allocator(const allocator& other) noexcept {};
	template<typename U>
	allocator(const allocator<U>& other) noexcept {};
	~allocator() {}

	T* address(T& x)const {
		return addressof(x);
	}
	const T* address(const T& x)const {
		return addressof(x);
	}

	T* allocate(std::size_t n = 1) {
		if (n == 0)
			return nullptr;
		return (T*)::operator new(n * sizeof(T));
	}

	void deallocate(T* ptr, std::size_t n) {
		::operator delete (ptr++);
	}

	template<class... Args>
	inline void construct(T* ptr, Args&&... args) {
		new (ptr) T(std::forward<Args>(args)...);
	}
	inline void destory(T* ptr) {
		ptr->~T();
	}

};

}

#endif // !ALLOCATOR_HPP