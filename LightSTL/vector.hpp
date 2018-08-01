#ifndef VECTOR_HPP
#define VECTOR_HPP


#include<utility>
#include"iterator/iterator.hpp"
#include"memory/uninitialized_storage.hpp"
#include"memory/allocator.hpp"
#include<initializer_list>
#include<type_traits>
#include<cassert>	
#include<stdexcept>

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
		std::pair<T*,T*> res = alloc_n_move(start, finish, newcap);
		_free_data();
		start = res.first;
		finish = res.second;
		end_of_storage = start + newcap;
	}

	void chk_n_alloc() {
		if (size() == capacity())
			_realocate();
	}

	std::pair<T*,T*> alloc_n_copy(T*b, T*e, size_type n) {
		assert( (e - b) <= n);
		T* p = data_alloc.allocate(n);
		return { p,LightSTL::uninitialized_copy(b, e, p) };
	}

	std::pair<T*,T*> alloc_n_move(T*b, T*e, size_type n) {
		assert((e - b) <= n);
		T* p = data_alloc.allocate(n);
		return { p,LightSTL::uninitialized_move(b,e,p) };
	}

	std::pair<T*,T*> alloc_n_fill(const T& val, size_type n) {
		T* p = data_alloc.allocate(n);
		return { p, LightSTL::uninitialized_fill_n(start, n, val) };
	}
public:

	vector()
		: start(nullptr), finish(nullptr), end_of_storage(nullptr) {}
	explicit vector(const Allocator& alloc)
		: start(nullptr), finish(nullptr), end_of_storage(nullptr),data_alloc(alloc) {}

	vector(size_type count, const T& value, const Allocator& alloc = Allocator())
		: data_alloc(alloc) {
		auto res = alloc_n_fill(value, count);
		start = res.first;
		finish = end_of_storage = res.second;
	}

	//委托构造函数
	explicit vector(size_type count, const Allocator& alloc = Allocator())
		: vector(count, T(), alloc)  {}


	template<class InputIt>
	vector(InputIt first, InputIt last, const Allocator& alloc = Allocator())
		: data_alloc(alloc) {
		std::pair<T*,T*> res = alloc_n_copy(first, last, last - last);
		start = res.first;
		finish = end_of_storage = res.second;
	}

	//委托构造函数
	vector(const vector& other)
		: vector(other, other.data_alloc) {}

	vector(const vector& other, const Allocator& alloc)
		: data_alloc(alloc) {
		size_type cap = other.capacity();
		auto res = alloc_n_copy(other.start, other.finish, cap);
		start = res.first;
		finish = res.second;
		end_of_storage = start + cap;
	}

	vector(std::initializer_list<T> init, const Allocator& alloc = Allocator()) {
		std::pair<T*, T*> res = alloc_n_copy(init.begin(), init.end(), init.end() - init.begin());
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
		auto res = alloc_n_copy(other.start, other.finish, cap);
		start = res.first;
		finish = res.second;
		end_of_storage = start + cap;
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
		auto b = ilist.begin(), e = ilist.end();
		std::pair<T*, T*> res = alloc_n_copy(b, e, e - b);
		start = res.first;
		finish = end_of_storage = res.second;
	}

public:
	//成员函数
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
		_free_data();
		auto res = alloc_n_fill(value, count);
		start = res.first;
		finish = end_of_storage = res.second;
	}

	template< class InputIt ,std::enable_if_t>
	void assign(InputIt first, InputIt last) {
		_free_data();
		auto res = alloc_n_copy(first, last, last - first);
		start = res.first;
		finish = end_of_storage = res.second;
	}

	void assign(std::initializer_list<T> ilist) {
		_free_data();
		auto b = ilist.begin(), e = ilist.end();
		auto res = alloc_n_copy(b, e, e - b);
		start = res.first;
		finish = end_of_storage = res.second;
	}

	reference at(size_type pos) {
		if (size() <= pos)
			throw std::out_of_range();
		return start[pos];
	}
	const_reference at(size_type pos) const {
		if (size() <= pos)
			throw std::out_of_range();
		return start[pos];
	}

	reference operator[](size_type pos) {
		return start[pos];
	}
	const_reference operator[](size_type pos) const {
		return start[pos];
	}

	reference front() {
		return *start;
	}
	const_reference front() const {
		return *start;
	}

	reference back() {
		return *(finish - 1);
	}
	const_reference back() const {
		return *(finish - 1);
	}

	T* data() noexcept {
		return start;
	}
	const T* data() const noexcept {
		return start;
	}

	iterator begin() noexcept {
		return start;
	}
	const_iterator begin() const noexcept {
		return start;
	}
	const_iterator cbegin() const noexcept {
		return start;
	}

	iterator end() {
		return finish;
	}
	const_iterator end() const noexcept {
		return finish;
	}
	const_iterator cend() const noexcept {
		return finish;
	}

	reverse_iterator rbegin() noexcept {
		return reverse_iterator(finish - 1);
	}
	const_reverse_iterator rbegin() const noexcept {
		return const_reverse_iterator(finish - 1);
	}
	const_reverse_iterator crbegin() const noexcept {
		return const_reverse_iterator(finish - 1);
	}

	reverse_iterator rend() noexcept {
		return reverse_iterator(start - 1);
	}
	const_reverse_iterator rend() const noexcept {
		return const_reverse_iterator(start - 1);
	}
	const_reverse_iterator crend() const noexcept {
		return const_reverse_iterator(start - 1);
	}

	bool empty() const noexcept {
		return start == nullptr;
	}

	void reserve(size_type new_cap) {
		if (new_cap > capacity()) {
			auto res = alloc_n_move(start, finish, new_cap);
			_free_data();
			start = res.first;
			finish = res.second;
			end_of_storage = start + new_cap;
		}
	}

	void clear() noexcept {
		erase(start, finish);
	}

	iterator insert(const_iterator pos, const T& value) {

	}
	iterator insert(const_iterator pos, T&& value) {

	}
	iterator insert(const_iterator pos, size_type count, const T& value) {

	}
	template< class InputIt >
	iterator insert(const_iterator pos, InputIt first, InputIt last) {

	}
	iterator insert(const_iterator pos, std::initializer_list<T> ilist) {

	}

	template< class... Args >
	iterator emplace(const_iterator pos, Args&&... args) {

	}
	iterator erase(const_iterator pos) {

	}
	iterator erase(const_iterator first, const_iterator last) {

	}

	void push_back(const T& value) {
		chk_n_alloc();
		data_alloc.construct(finish++, value);
	}
	void push_back(T&& value) {
		chk_n_alloc();
		data_alloc.construct(finish++, std::move(value));
	}

	template< class... Args >
	reference emplace_back(Args&&... args) {
		chk_n_alloc();
		data_alloc.construct(std::forward<Args>(args)...);
	}

	void pop_back() {

	}

	void resize(size_type count) {

	}
	void resize(size_type count, const value_type& value) {

	}
};




#endif // !VECTOR_HPP