#ifndef VECTOR_HPP
#define VECTOR_HPP


#include<utility>
#include"iterator/iterator.hpp"
#include"memory/uninitialized_storage.hpp"
#include"memory/allocator.h"
#include<initializer_list>
#include<type_traits>

template<class T, class Allocator = LightSTL::allocator<T> >
class vector {
public:
	using value_type = T;
	using allocator_type = Allocator;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = T & ;
	using const_reference = const T&;
	using pointer = T * ;
	using const_pointer = const T*;
	using iterator = T * ;
	using const_iterator = const T*;
	using reverse_iterator = LightSTL::reverse_iterator<iterator>;
	using const_reverse_iterator = LightSTL::reverse_iterator<const_iterator>;
private:
	iterator start;
	iterator finish;
	iterator end_of_storage;
	LightSTL::allocator<T>data_alloc;

	void _free_data() {
		if (start) {
			for (auto i = finish; i != start; --i)
				data_alloc.destory(i);
			data_alloc.deallocate(start, end_of_storage - start);
		}
	}

	void _realocate() {
		size_type newcap = size() ? size() * 2 : 1;
		T* newdata = data_alloc.allocate(newcap);
		iterator old_start = start;
		T* new_start = newdata;
		size_type old_size = size();
		for (size_type i = 0; i < old_size; ++i)
			data_alloc.construct(old_start++, std::move(*new_start++));
		_free_data();
		start = newdata;
		end_of_storage = new_start + newcap;
		finish = new_start;
	}

	void chk_n_alloc() {
		if (size() == capacity())
			_realocate();
	}

	void alloc_n_copy(const T*b, const T*e, size_type n) {
		T* p = data_alloc.allocate(n);
		start = p;
		finish = LightSTL::uninitialized_copy(b, e, p);
		end_of_storage = start + n;
	}

public:

	vector()
		: start(nullptr), finish(nullptr), end_of_storage(nullptr) {}
	explicit vector(const Allocator& alloc)
		: start(nullptr), finish(nullptr), end_of_storage(nullptr),data_alloc(alloc) {}

	vector(size_type count, const T& value, const Allocator& alloc = Allocator())
		: data_alloc(alloc) {
		T* p = data_alloc.allocate(count + 3);
		start = p;
		for (size_type i = 0; i < count; ++i)
			data_alloc.construct(p++, value);
		finish = p;
		end_of_storage = start + count + 3;
	}

	//委托构造函数
	explicit vector(size_type count, const Allocator& alloc = Allocator())
		: vector(count, T(), alloc)  {}


	template<class InputIt>
	vector(InputIt first, InputIt last, const Allocator& alloc = Allocator())
		: data_alloc(alloc) {
		std::pair<T*,T*> res = alloc_n_copy(first, last);
		start = res.first;
		finish = end_of_storage = res.second;
	}

	vector(const vector& other)
		: data_alloc(other.data_alloc) {
		size_type cap = other.capacity();
		T* p = data_alloc.allocate(cap);
		start = p;
		finish = LightSTL::uninitialized_copy(other.start, other.finish, p);
		end_of_storage = p + cap;
	}

	vector(const vector& other, const Allocator& alloc)
		: data_alloc(alloc) {
		size_type cap = other.capacity();
		T* p = data_alloc.allocate(cap);
		start = p;
		finish = LightSTL::uninitialized_copy(other.start, other.finish, p);
		end_of_storage = p + cap;
	}

	vector(std::initializer_list<T> init, const Allocator& alloc = Allocator()) {
		std::pair<T*, T*> res = alloc_n_copy(init.begin(), init.end());
		start = res.first;
		finish = end_of_storage = res.second;
	}

	//移动构造函数
	vector(vector&& other)noexcept
		: start(other.start), finish(other.start), end_of_storage(other.end_of_storage), data_alloc(std::move(other.data_alloc)) {
		other.start = nullptr;
		other.finish = nullptr;
		other.end_of_storage = nullptr;
	}
	vector(vector&& other, const Allocator& alloc)
		: start(other.start), finish(other.finish), end_of_storage(other.end_of_storage), data_alloc(std::move(alloc)) {
		other.start = nullptr;
		other.finish = nullptr;
		other.end_of_storage = nullptr;
	}

	//析构函数
	~vector() {
		_free_data();
	}

	//赋值函数
	vector& operator=(const vector& other) {
		if (this == &other)
			return *this;
		_free_data();
		data_alloc = other.data_alloc;
		size_type cap = other.capacity();
		T* p = data_alloc.allocate(cap);
		start = p;
		finish = LightSTL::uninitialized_copy(other.start, other.finish, p);
		end_of_storage = p + cap;
	}

	vector& operator=(vector&& other) {
		if (this == &other)
			return *this;
		_free_data();
		data_alloc = std::move(other.data_alloc);
		start = other.start;
		finish = other.finish;
		end_of_storage = other.end_of_storage;
		other.start = nullptr;
		other.end_of_storage = nullptr;
		other.finish = nullptr;
	}

	vector& operator=(std::initializer_list<T> ilist) {
		_free_data();
		std::pair<T*, T*> res = alloc_n_copy(init.begin(), init.end());
		start = res.first;
		finish = end_of_storage = res.second;
	}

public:
	//成员函数
	iterator begin() {
		return start;
	}

	iterator end() {
		return finish;
	}

	size_type size()const {
		return finish - start;
	}

	size_type capacity()const {
		return end_of_storage - start;
	}

	allocator_type get_allocator() const {
		return data_alloc;
	}

	void assign(size_type count, const T& value) {

	}

	template< class InputIt ,std::enable_if_t>
	void assign(InputIt first, InputIt last) {

	}

	void assign(std::initializer_list<T> ilist) {

	}
};




#endif // !VECTOR_HPP