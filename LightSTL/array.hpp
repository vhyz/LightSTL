#ifndef ARRAY_HPP
#define ARRAY_HPP

#include<cstddef>
#include"iterator/iterator.hpp"
#include<stdexcept>
#include"memory/uninitialized_storage.hpp"


namespace LightSTL {

template<class T, std::size_t N>
struct array {
public:
	//typedef
	using value_type = T;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = value_type & ;
	using const_reference = const value_type&;
	using pointer = value_type * ;
	using const_pointer = const value_type*;
	using iterator = T * ;
	using const_iterator = const T*;
	using reverse_iterator = LightSTL::reverse_iterator<iterator>;
	using const_reverse_iterator = LightSTL::reverse_iterator<const_iterator>;


private:
	T datas[N];
public:
	//构造函数 析构函数 赋值函数 隐式由编译器生成


	//元素访问
	constexpr reference at(size_type pos) {
		if (pos >= N)
			throw std::out_of_range();
		return datas[pos];
	}
	constexpr const_reference at(size_type pos) const {
		if (pos >= N)
			throw std::out_of_range();
		return datas[pos];
	}
	constexpr reference operator[](size_type pos)                { return datas[pos];}
	constexpr const_reference operator[](size_type pos) const    { return datas[pos];}
	constexpr reference front()                                  { return datas[0];}
	constexpr const_reference front() const                      { return datas[0];}
	constexpr reference back()                                   { return datas[N - 1];}
	constexpr const_reference back() const                       { return datas[N - 1];}
	constexpr T* data() noexcept                                 { return datas;}
	constexpr const T* data() const noexcept                     { return datas;}

	//迭代器     
	constexpr iterator begin() noexcept                          { return datas;}
	constexpr const_iterator begin() const noexcept              { return datas; }
	constexpr const_iterator cbegin() const noexcept             { return datas; }
	constexpr iterator end() noexcept                            { return datas + N; }
	constexpr const_iterator end() const noexcept                { return datas + N; }
	constexpr const_iterator cend() const noexcept               { return datas + N; }
	constexpr reverse_iterator rbegin() noexcept                 { return end() - 1; }
	constexpr const_reverse_iterator  rbegin() const noexcept    { return end() - 1; }
	constexpr const_reverse_iterator crbegin() const noexcept    { return end() - 1; }
	constexpr reverse_iterator rend() noexcept                   { return begin() - 1; }
	constexpr const_reverse_iterator rend() const noexcept       { return begin() - 1; }
	constexpr const_reverse_iterator crend() const noexcept      { return begin() - 1; }

	//容量
	constexpr bool empty() const noexcept              { return begin() == end(); }
	size_type size() const noexcept                    { return N; }
	constexpr size_type max_size()const   noexcept     { return N; }

	//操作
	void fill(const T& value) {
		LightSTL::uninitialized_fill_n(datas, N, value);
	}

	void swap(array& other) noexcept {
		for (size_type i = 0; i < N; ++i) {
			T tmp = datas[i];
			datas[i] = other[i];
			other[i] = tmp;
		}
	}
};

template< class T, std::size_t N >
constexpr bool operator==(const array<T, N>& lhs,const array<T, N>& rhs) {
	for (std::size_t i = 0; i < N; ++i)
		if (lhs[i] != rhs[i])
			return false;
	return true;
}

template< class T, std::size_t N >
bool operator!=(const array<T, N>& lhs,const array<T, N>& rhs) {
	return !(lhs == rhs);
}


}

#endif // !ARRAY_HPP
