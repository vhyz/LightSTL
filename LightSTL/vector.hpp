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



namespace LightSTL {

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
	Allocator data_alloc;

	void _free_data() {
		if (start) {
			for (auto i = start; i != finish; ++i)
				LightSTL::destroy_at(i);
			data_alloc.deallocate(start, size());
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
		return { p, LightSTL::uninitialized_fill_n(p, n, val) };
	}

	void move_safe(T* first, T* last, T* d_first) {
		size_type len = last - first;
		d_first += (len - 1);
		last--;
		for (size_type i = 0; i < len; ++i, d_first--, last--) {
			data_alloc.construct(d_first, std::move(*last));
			data_alloc.destory(last);
		}
	}
	template<class It>
	iterator insert_aux_iterator(const_iterator pos, It first, It last) {
		
		//以一组迭代器方式进行插入
		auto dis = LightSTL::distance(first, last);
		size_type new_size = size() + dis;
		iterator old_pos = const_cast<iterator>(pos);

		if (new_size > capacity()) {
			//若空间不足，申请new_size的空间
			T* p = data_alloc.allocate(new_size);
			auto new_pos = uninitialized_move(start, old_pos, p);
			iterator res = uninitialized_copy(first, last, new_pos);
			uninitialized_move(old_pos, finish, res);
			_free_data();
			start = p;
			finish = end_of_storage = p + new_size;
			return new_pos;
		} else {
			//进行安全移动，从末尾元素开始移动
			move_safe(old_pos, finish, old_pos + dis);
			uninitialized_copy(first, last, old_pos);
			finish += dis;
			return old_pos;
		}

	}

	iterator insert_aux_n(const_iterator pos, size_type n, const T& value) {
		size_type pos_dis = pos - start, len = size();
		size_type new_size = n + len;
		iterator old_pos = const_cast<iterator>(pos);
		if (new_size > capacity()) {

			iterator p = data_alloc.allocate(new_size);
			iterator new_pos = uninitialized_move(start, old_pos, p);
			iterator res = uninitialized_fill_n(new_pos, n, value);
			uninitialized_move(old_pos, finish, res);
			_free_data();
			start = p;
			finish = end_of_storage = p + new_size;
			return new_pos;
		} else {

			move_safe(old_pos, finish, old_pos + n);
			uninitialized_fill_n(old_pos, n, value);
			finish += n;
			return old_pos;
		}
	}

	iterator insert_aux_n_default(const_iterator pos, size_type n) {
		size_type pos_dis = pos - start, len = size();
		size_type new_size = n + len;
		iterator old_pos = const_cast<iterator>(pos);
		if (new_size > capacity()) {

			iterator p = data_alloc.allocate(new_size);
			iterator new_pos = uninitialized_move(start, old_pos, p);
			iterator res = uninitialized_default_construct_n(new_pos, n);
			uninitialized_move(old_pos, finish, res);
			_free_data();
			start = p;
			finish = end_of_storage = p + new_size;
			return new_pos;
		} else {

			move_safe(old_pos, finish, old_pos + n);
			uninitialized_default_construct_n(old_pos, n);
			finish += n;
			return old_pos;
		}
	}
	template<class... Args>
	iterator insert_aux_args(const_iterator pos, Args&&... args) {
		size_type pos_dis = pos - start;
		size_type len = size();
		iterator old_pos = const_cast<iterator>(pos);
		if (finish == end_of_storage) {
			size_type new_size = len ? len * 2 : 1;
			T* p = data_alloc.allocate(new_size);
			auto new_pos = uninitialized_move(start, old_pos, p);
			data_alloc.construct(new_pos, std::forward<Args>(args)...);
			auto new_finish = uninitialized_move(old_pos, finish, new_pos + 1);
			_free_data();
			start = p;
			finish = new_finish;
			end_of_storage = p + new_size;
			return new_pos;
		} else {
			uninitialized_move(old_pos, finish, old_pos + 1);
			finish++;
			data_alloc.construct(old_pos, std::forward<Args>(args)...);
			return old_pos;
		}
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
		: data_alloc(alloc) {
		T* p = data_alloc.allocate(count);
		start = p;
		end_of_storage = finish = LightSTL::uninitialized_default_construct_n(p, count);
	}


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
		std::pair<T*, T*> res = alloc_n_copy(const_cast<T*>(init.begin()), const_cast<T*>(init.end()), init.end() - init.begin());
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
	//容量
	size_type size()const { return finish - start;}

	size_type capacity()const {  return end_of_storage - start; }

	allocator_type get_allocator() const { return data_alloc;}

	void assign(size_type count, const T& value) {
		_free_data();
		auto res = alloc_n_fill(value, count);
		start = res.first;
		finish = end_of_storage = res.second;
	}

	template< class InputIt>
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


	//元素访问
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
	reference operator[](size_type pos)             { return start[pos];}
	const_reference operator[](size_type pos) const { return start[pos];}
	reference front()                               { return *start;}
	const_reference front() const                   { return *start;}
	reference back()                                { return *(finish - 1);}
	const_reference back() const                    { return *(finish - 1);}
	T* data() noexcept                              { return start; }
	const T* data() const noexcept                  { return start; }


	//迭代器
	iterator begin() noexcept                       { return start; }
	const_iterator begin() const noexcept           { return start; }
	const_iterator cbegin() const noexcept          { return start; }
	iterator end()                                  { return finish;}
	const_iterator end() const noexcept             { return finish;}
	const_iterator cend() const noexcept            { return finish;}
	reverse_iterator rbegin() noexcept              { return reverse_iterator(finish);      }
	const_reverse_iterator rbegin() const noexcept  { return const_reverse_iterator(finish);}
	const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(finish);}
	reverse_iterator rend() noexcept                { return reverse_iterator(start);       }
	const_reverse_iterator rend() const noexcept    { return const_reverse_iterator(start);}
	const_reverse_iterator crend() const noexcept   { return const_reverse_iterator(start); }

	bool empty() const noexcept {
		return size() == 0;
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
		return insert_aux_args(pos, value);
	}
	iterator insert(const_iterator pos, T&& value) {
		return insert_aux_args(pos, value);
	}
	iterator insert(const_iterator pos, size_type count, const T& value) {
		return insert_aux_n(pos, count, value);
	}
	iterator insert(const_iterator pos, size_type count) {
		return insert_aux_n_default(pos, count);
	}
	template< class InputIt >
	iterator insert(const_iterator pos, InputIt first, InputIt last) {
		return insert_aux_iterator(pos, first, last);
	}
	iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
		return insert_aux_iterator(pos, ilist.begin(), ilist.end());
	}

	template< class... Args >
	iterator emplace(const_iterator pos, Args&&... args) {
		return insert_aux_args(pos, std::forward<Args>(args)...);
	}
	iterator erase(const_iterator pos) {
		return erase(pos, pos + 1);
	}
	iterator erase(const_iterator first, const_iterator last) {
		for (; first < last; ++first)
			first->~T();
		if (last != finish)
			finish = LightSTL::uninitialized_move(const_cast<iterator>(last), finish, const_cast<iterator>(first));
		else
			finish = const_cast<iterator>(first);
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
		data_alloc.construct(finish++, std::forward<Args>(args)...);
		return *(finish - 1);
	}

	void pop_back() {
		data_alloc.destory(--finish);
	}

	void resize(size_type count) {
		if (count > capacity()) {
			auto res = alloc_n_move(start, finish, count);
			start = res.first;
			finish = res.second;
			end_of_storage = start + count;
		}

		if (count > size()) {
			finish = LightSTL::uninitialized_default_construct_n(finish, count - size());
		} else {
			size_type len = size() - count;
			for (size_type i = 0; i < len; ++i)
				data_alloc.destory(finish--);
		}
	}

	void resize(size_type count, const value_type& value) {
		if (count > capacity()) {
			auto res = alloc_n_move(start, finish, count);
			start = res.first;
			finish = res.second;
			end_of_storage = start + count;
		}

		if (count > size()) {
			finish = LightSTL::uninitialized_fill_n(finish, count - size(), value);
		} else {
			size_type len = size() - count;
			for (size_type i = 0; i < len; ++i)
				data_alloc.destory(finish--);
		}
	}
};

template< class T, class Alloc >
bool operator==(const vector<T, Alloc>& lhs,
	const vector<T, Alloc>& rhs) {
	if (lhs.size() != rhs.size())
		return false;
	size_t len = lhs.size();
	for (size_t i = 0; i < len; ++i)
		if (lhs[i] != rhs[i])
			return false;
	return true;
}
template< class T, class Alloc >
bool operator!=(const vector<T, Alloc>& lhs,
	const vector<T, Alloc>& rhs) {
	return !(rhs == lhs);
}
template< class T, class Alloc >
bool operator<(const vector<T, Alloc>& lhs,
	const vector<T, Alloc>& rhs) {
	if (lhs.size() >= rhs.size())
		return false;
	size_t len = lhs.size();
	for (size_t i = 0; i < len; ++i)
		if (lhs[i] >= rhs[i])
			return false;
	return true;
}
template< class T, class Alloc >
bool operator>(const vector<T, Alloc>& lhs,
	const vector<T, Alloc>& rhs) {
	if (lhs.size() <= rhs.size())
		return false;
	size_t len = lhs.size();
	for (size_t i = 0; i < len; ++i)
		if (lhs[i] <= rhs[i])
			return false;
	return true;
}
template< class T, class Alloc >
bool operator<=(const vector<T, Alloc>& lhs,
	const vector<T, Alloc>& rhs) {
	return !(lhs > rhs);
}

template< class T, class Alloc >
bool operator>=(const vector<T, Alloc>& lhs,
	const vector<T, Alloc>& rhs) {
	return !(lhs < rhs);
}




}

#endif // !VECTOR_HPP