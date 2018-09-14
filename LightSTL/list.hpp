#ifndef LIST_HPP
#define LIST_HPP


#include<cstddef>
#include"memory/allocator.hpp"
#include"iterator/iterator.hpp"
#include"memory/addressof.hpp"
#include<memory>



namespace LightSTL {




namespace detail {
	template<class T>
	struct node {
		node* prev;
		node* next;
		T data;

		template<class... Args>
		node(node* p, node* n, Args&&... args)
			: prev(p), next(n), data(std::forward<Args>(args)...) {
		}
	};

	template<class T>
	struct remove_const {
		using value = T;
	};

	template<class T>
	struct remove_const<const T> {
		using value = T;
	};

	template<class T>
	class list_iterator {
		template<class T>
		friend typename list_iterator<T>::list_node* get_node(list_iterator<T> it);

		template<class Y>
		friend class list_iterator;
	public:
		//typedefs
		using iterator_category = LightSTL::bidirectional_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T * ;
		using reference = T & ;
	private:
		using element_type = typename remove_const<T>::value;
		using list_node = node<element_type>;

		list_node* n;

	public:

		list_iterator(list_node* x)
			: n(x)  {}

		template<class Y>
		list_iterator(const list_iterator<Y>& other)
			: n(other.n)  {}

		T& operator*()const  { return n->data; }
		T* operator->()const { return LightSTL::addressof(n->data); }

		list_iterator& operator++() {
			n = n->next;
			return *this;
		}

		list_iterator operator++(int) {
			list_iterator res = *this;
			n = n->next;
			return res;
		}
		
		list_iterator operator--() {
			n = n->prev;
			return *this;
		}

		list_iterator operator--(int) {
			list_iterator res = *this;
			n = n->prev;
			return res;
		}

		bool operator==(const list_iterator& other)const { return n == other.n; }
		bool operator!=(const list_iterator& other)const { return n != other.n; }

		
	};

	template<class T>
	typename list_iterator<T>::list_node* get_node(list_iterator<T> it) {
		return it.n;
	}
}

template<class T,class Allocator = LightSTL::allocator<T> >
class list {

public:
	//typedefs
	using value_type = T;
	using allocator_type = Allocator;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = T & ;
	using const_reference = const T&;
	using pointer = typename Allocator::pointer;
	using const_pointer = typename Allocator::const_pointer;
	using iterator = detail::list_iterator<T>;
	using const_iterator = detail::list_iterator<const T>;
	using reverse_iterator = LightSTL::reverse_iterator<iterator>;
	using const_reverse_iterator = LightSTL::reverse_iterator<const_iterator>;

private:

	using node = detail::node<T>;
	using node_allocator = typename Allocator::rebind<node>::other;

	template<class... Args>
	node* create_node(Args&&... args) {
		node* res = data_alloc.allocate();
		data_alloc.construct(res, std::forward<Args>(args)...);
		return res;
	}

	void destory_node(node* n) {
		data_alloc.destory(n);
		data_alloc.deallocate(n, 1);
	}

	void empty_init() {
		_node = data_alloc.allocate(1);
		_node->next = _node;
		_node->prev = _node;
		_size = 0;
	}

	void free_nodes() {
		node* cur = _node->next;
		for (size_type i = 0; i < _size; ++i) {
			node*tmp = cur->next;
			destory_node(cur);
			cur = tmp;
		}
		_size = 0;
		_node->next = _node;
		_node->prev = _node;
	}

	node* _node;
	size_type _size;
	node_allocator data_alloc;

private:
	//四种插入辅助函数
	template<class It>
	iterator insert_aux_iterator(const_iterator pos, It start, It last);
	template<class... Args>
	iterator insert_aux_args(const_iterator pos, Args&&... args);
	iterator insert_aux_n(const_iterator pos, size_type n, const T& val);
	iterator insert_aux_n_default(const_iterator pos, size_type n);

private:
	void copy_list(const list& other) {
		insert_aux_iterator(end(), other.begin(), other.end());
	}

	//转移资源时使用函数
	void _resert(node* n) {
		_node = n;
		_size = 0;
	}

	template<class T>
	void _swap(T& lhs, T&rhs) {
		T tmp = lhs;
		lhs = rhs;
		rhs = tmp;
	}


private:

	iterator _erase_n(const_iterator s, const_iterator l) {
		node* start = detail::get_node(s), last = detail::get_node(l);
		last->prev = start->prev;
		start->prev->next = last;
		while (start != last) {
			node*next = start->next;
			destory_node(start);
			start = next;
			_size -= 1;
		}
		return iterator(last);
	}

	iterator _erase(const_iterator it) {
		node* n = detail::get_node(it), *res = n->next;
		n->prev->next = res;
		res->prev = n->prev;
		destory_node(n);
		_size -= 1;
		return iterator(res);
	}

	void move_aux(list&& other) {
		node*tmp = other._node;
		_size = other._size;
		other._resert(_node);
		_node = tmp;
	}
public:
	//构造函数
	list() { 
		empty_init(); 
	}

	explicit list(const Allocator& alloc) 
		: data_alloc(alloc) { 
		empty_init();
	}

	list(size_type count, const T& value, const Allocator& alloc = Allocator())
		: data_alloc(alloc) {
		empty_init();
		insert_aux_n(end(), count, value);
	}

	explicit list(size_type count, const Allocator& alloc = Allocator())
		: data_alloc(alloc) {
		empty_init();
		insert_aux_n_default(end(), count);
	}

	template< class InputIt >
	list(InputIt first, InputIt last, const Allocator& alloc = Allocator())
		: data_alloc(alloc) {
		empty_init();
		insert_aux_iterator(end(), first, last);
	}


	//委托构造函数
	list(const list& other)
		: list(other,other.data_alloc)  {}

	list(const list& other, const Allocator& alloc)
		: data_alloc(alloc) {
		empty_init();
		copy_list(other);
	}

	list(list&& other)
		:  data_alloc(std::move(other.data_alloc)) {
		empty_init();
		move_aux(std::move(other));
	}

	list(list&& other, const Allocator& alloc)
		: data_alloc(alloc) {
		empty_init();
		move_aux(std::move(other));
	}

	list(std::initializer_list<T> init, const Allocator& alloc = Allocator()) {
		empty_init();
		insert_aux_iterator(end(), init.begin(), init.end());
	}

	//析构函数
	~list() {
		free_nodes();
		data_alloc.deallocate(_node,1);
	}


	//复值函数
	list& operator=(const list& other) {
		if (&other == this)
			return *this;
		free_nodes();
		copy_list(other);
	}

	list& operator=(list&& other) noexcept {
		if (&other == this)
			return *this;
		data_alloc = std::move(other.data_alloc);
		free_nodes();
		move_aux(std::move(other));
	}

	list& operator=(std::initializer_list<T> ilist) {
		free_nodes();
		insert_aux_iterator(end(), ilist.begin(), ilist.end());
	}

	void assign(size_type count, const T& value) {
		free_nodes();
		insert_aux_n(end(), count, value);
	}

	template< class InputIt >
	void assign(InputIt first, InputIt last) {
		free_nodes();
		insert_aux_iterator(end(), first, last);
	}

	void assign(std::initializer_list<T> ilist) {
		free_nodes();
		insert_aux_iterator(end(), ilist.begin(), ilist.end());
	}

	allocator_type get_allocator() const { return data_alloc; }

	//元素访问
	reference front() { return *begin();}
	const_reference front() const { return *begin(); }
	reference back() {
		iterator it = end();
		it--;
		return *it;
	}
	const_reference back() const {
		iterator it = end();
		it--;
		return *it;
	}

	//迭代器
	iterator begin() noexcept {
		iterator it = end();
		++it;
		return it;
	}
	const_iterator begin() const noexcept {
		return cbegin();
	}
	const_iterator cbegin() const noexcept {
		const_iterator it = cend();
		++it;
		return it;
	}
	iterator end() noexcept {
		return iterator(_node);
	}
	const_iterator end() const noexcept {
		return cend();
	}
	const_iterator cend() const noexcept {
		return const_iterator(_node);
	}
	reverse_iterator rbegin() noexcept {
		return reverse_iterator(end());
	}
	const_reverse_iterator rbegin() const noexcept {
		return const_reverse_iterator(cend());
	}
	const_reverse_iterator crbegin() const noexcept {
		return const_reverse_iterator(cend());
	}
	reverse_iterator rend() noexcept {
		return reverse_iterator(begin());
	}
	const_reverse_iterator rend() const noexcept {
		return const_reverse_iterator(cbegin());
	}
	const_reverse_iterator crend() const noexcept {
		return const_reverse_iterator(cbegin());
	}
	
	//容量
	bool empty() const noexcept { return _size == 0; }
	size_type size() const noexcept { return _size; }

	//修改器
	void clear() noexcept { 
		free_nodes(); 
	}

	void push_back(const T& value) {
		insert_aux_args(end(), value);
	}

	void push_back(T&& value) {
		insert_aux_args(end(), std::move(value));
	}

	template< class... Args >
	reference emplace_back(Args&&... args) {
		insert_aux_args(end(), std::forward<Args>(args)...);
	}

	void pop_back() {
		iterator it = end();
		it--;
		_erase(it);
	}

	void push_front(const T& value) {
		insert_aux_args(begin(), value);
	}

	void push_front(T&& value) {
		insert_aux_args(begin(), std::move(value));
	}

	template< class... Args >
	reference emplace_front(Args&&... args) {
		insert_aux_args(begin(), std::forward<Args>(args)...);
	}

	void pop_front() {
		_erase(begin());
	}

	iterator erase(const_iterator pos) {
		return _erase(pos);
	}

	iterator erase(const_iterator first, const_iterator last) {
		return _erase_n(first, last);
	}

	iterator insert(const_iterator pos, const T& value) {
		return insert_aux_args(pos, value);
	}

	iterator insert(const_iterator pos, T&& value) {
		return insert_aux_args(pos, std::move(value));
	}

	iterator insert(const_iterator pos, size_type count, const T& value) {
		return insert_aux_n(pos, count, value);
	}

	template< class InputIt >
	iterator insert(const_iterator pos, InputIt first, InputIt last) {
		return insert_aux_iterator(pos, first, last);
	}

	iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
		return insert_aux_iterator(pos, ilist.begin(), ilist.end());
	}

	void resize(size_type count) {
		if (count < _size) {
			iterator last = end();
			iterator first = last;
			while (count--) {
				--first;
			}
			erase(first, last);
		} else {
			insert_aux_n_default(end(), count - _size);
		}
	}

	void resize(size_type count, const value_type& value) {
		if (count < _size) {
			iterator last = end();
			iterator first = last;
			while (count--) {
				--first;
			}
			erase(first, last);
		} else {
			insert_aux_n(end(), count - _size, value);
		}
	}

	void swap(list& other) noexcept {
		_swap(_node, other._node);
		_swap(_size, other._size);
	}

};


template< class T, class Alloc >
bool operator==(const LightSTL::list<T, Alloc>& lhs, const LightSTL::list<T, Alloc>& rhs) {
	if (lhs.size() != rhs.size())
		return false;
	auto lhs_begin = lhs.begin(), rhs_begin = rhs.begin();
	size_t len = lhs.size();
	for (size_t i = 0; i < len; ++i, ++lhs_begin, ++rhs_begin)
		if (*lhs != *rhs)
			return false;
	return true;
}
template< class T, class Alloc >
bool operator!=(const LightSTL::list<T, Alloc>& lhs, const LightSTL::list<T, Alloc>& rhs) {
	return !(lhs == rhs);
}
template< class T, class Alloc >
bool operator<(const LightSTL::list<T, Alloc>& lhs, const LightSTL::list<T, Alloc>& rhs) {
	if (lhs.size() < rhs.size())
		return true;
	else if (lhs.size() > rhs.size())
		return false;
	else {
		auto lhs_begin = lhs.begin(), rhs_begin = rhs.begin();
		size_t len = lhs.size();
		for (size_t i = 0; i < len; ++i, ++lhs_begin, ++rhs_begin)
			if (*lhs >= *rhs)
				return false;
		return true;
	}
}
template< class T, class Alloc >
bool operator>(const LightSTL::list<T, Alloc>& lhs, const LightSTL::list<T, Alloc>& rhs) {
	if (lhs.size() > rhs.size())
		return true;
	else if (lhs.size() < rhs.size())
		return false;
	else {
		auto lhs_begin = lhs.begin(), rhs_begin = rhs.begin();
		size_t len = lhs.size();
		for (size_t i = 0; i < len; ++i, ++lhs_begin, ++rhs_begin)
			if (*lhs <= *rhs)
				return false;
		return true;
	}
}
template< class T, class Alloc >
bool operator<=(const LightSTL::list<T, Alloc>& lhs, const LightSTL::list<T, Alloc>& rhs) {
	return !(lhs > rhs);
}
template< class T, class Alloc >
bool operator>=(const LightSTL::list<T, Alloc>& lhs, const LightSTL::list<T, Alloc>& rhs) {
	return !(lhs < rhs);
}





template<class T, class Allocator>
template<class It>
inline typename list<T, Allocator>::iterator list<T, Allocator>::insert_aux_iterator(const_iterator pos, It start, It last) {
	node *pre, *cur;
	node* pos_address = detail::get_node(pos);
	pre = pos_address->prev;
	node* res_pre = pre;
	for (; start != last; ++start) {
		cur = create_node(pre, pos_address, *start);
		pre->next = cur;
		pre = cur;
		_size += 1;
	}
	return res_pre->next;
}

template<class T, class Allocator>
template<class ...Args>
inline typename list<T, Allocator>::iterator list<T, Allocator>::insert_aux_args(const_iterator pos, Args && ...args) {
	node* pos_address = detail::get_node(pos);
	node* pre = pos_address->prev;
	pre->next = create_node(pre, pos_address, std::forward<Args>(args)...);
	_size += 1;
	return pre->next;
}

template<class T, class Allocator>
inline typename list<T, Allocator>::iterator list<T, Allocator>::insert_aux_n(const_iterator pos, size_type n, const T & val) {
	node *pre, *cur;
	node* pos_address = detail::get_node(pos);
	pre = pos_address->prev;
	node* res_pre = pre;
	while (n--) {
		cur = create_node(pre, pos_address, val);
		pre->next = cur;
		pre = cur;
		_size += 1;
	}
	return res_pre->next;
}

template<class T, class Allocator>
inline typename list<T, Allocator>::iterator LightSTL::list<T, Allocator>::insert_aux_n_default(const_iterator pos, size_type n) {
	node  pre, *cur;
	node* pos_address = detail::get_node(pos);
	pre = pos_address->prev;
	node* res_pre = pre;
	while (n--) {
		cur = create_node(pre, pos_address);
		pre->next = cur;
		pre = cur;
		_size += 1;
	}
	return res_pre->next;
}

}

#endif // !LIST_HPP