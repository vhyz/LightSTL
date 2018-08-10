#ifndef LIST_HPP
#define LIST_HPP


#include<cstddef>
#include"memory/allocator.hpp"
#include"iterator/iterator.hpp"


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
	class list_iterator: public LightSTL::iterator<LightSTL::bidirectional_iterator_tag,T> {
	private:

		using list_node = node<T>;

		list_node* n;

	public:

		list_iterator(list_node* x)
			: n(x)  {}

		list_iterator(const list_iterator& other)
			: n(other.n)  {}

		T& operator*()const  { return n->data; }
		T* operator->()const { return &(n->data); }

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
	using pointer = Allocator::pointer;
	using const_pointer = Allocator::const_pointer;
	using iterator = detail::list_iterator<T>;
	using const_iterator = detail::list_iterator<const T>;
	using reverse_iterator = LightSTL::reverse_iterator<iterator>;
	using const_reverse_iterator = LightSTL::reverse_iterator<const_iterator>;

private:

	using node = detail::node<T>;

	template<class... Args>
	node* create_node(Args&&... args) {
		node* res = data_alloc.allocate();
		data_alloc.construct(res, std::forward<Args>(args)...);
		return res;
	}

	void destory_node(node* n) {
		data_alloc.destory(n);
		data_alloc.deallocate(n);
	}

	void empty_init() {
		_node = data_alloc.allocate();
		_node->next = _node;
		_node->prev = _node;
		_size = 0;
	}

	void free_nodes() {
		for (size_type i = 0; i < _size; ++i) {
			node*tmp = _node->next;
			destory_node(_node);
			_node = tmp;
		}
		_size = 0;
	}
	node* _node;
	size_type _size;
	LightSTL::allocator<T> data_alloc;

	template<class It>
	void insert_aux_iterator(it start, it last) {

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
		: data_alloc(alloc) {}

	explicit list(size_type count, const Allocator& alloc = Allocator())
		: data_alloc(alloc) {
	}

	template< class InputIt >
	list(InputIt first, InputIt last,
		const Allocator& alloc = Allocator());

	//委托构造函数
	list(const list& other)
		: list(other,other.data_alloc)  {}

	list(const list& other, const Allocator& alloc)
		: data_alloc(alloc) {

	}

	list(list&& other)
		: _node(other._node), _size(other._size), data_alloc(std::move(other.data_alloc)) {}

	list(list&& other, const Allocator& alloc)
		: _node(other._node), _size(other._size), data_alloc(alloc) {}


	list(std::initializer_list<T> init, const Allocator& alloc = Allocator());

	//析构函数
	~list() {
		free_nodes();
	}




	//元素访问
	reference front() { return _head->data; }
	const_reference front() const { return _head->data; }
	reference back() { return _back->data; }
	const_reference back() const { return _back->data; }

	//迭代器
	iterator begin() noexcept { return iterator(_head); }
	const_iterator begin() const noexcept { return const_iterator(_head); }
	const_iterator cbegin() const noexcept { return const_iterator(_head); }
	iterator end() noexcept { return iterator(nullptr); }
	const_iterator end() const noexcept { return const_iterator(nullptr); }
	const_iterator cend() const noexcept { return const_iterator(nullptr); }
	reverse_iterator rbegin() noexcept { return reverse_iterator(_back); }
	const_reverse_iterator rbegin() const noexcept;
	const_reverse_iterator crbegin() const noexcept;
	reverse_iterator rend() noexcept;
	const_reverse_iterator rend() const noexcept;
	const_reverse_iterator crend() const noexcept;
	
	//容量
	bool empty() const noexcept { return _size == 0; }
	size_type size() const noexcept { return _size; }

	//修改器
	void clear() noexcept { 
		free_nodes(); 
	}
	void push_back(const T& value) {
		if (_size) {
			_back = create_node(_back, nullptr, value);
		} else {
			create_first_node(value);
		}
		_size++;
	}
	void push_back(T&& value) {
		if (_size) {
			_back = create_node(_back, nullptr, std::move(value));
		} else {
			create_first_node(std::move(value));
		}
		_size++;
	}
	template< class... Args >
	reference emplace_back(Args&&... args) {
		if (_size) {
			_back = create_first_node(_back, nullptr, std::forward<Args>(args)...);
		} else {
			create_first_node(std::forward<Args>(args)...);
		}
		_size++;
	}
	void pop_back() {
		_size--;
		node* prev = _back->prev;
		destory_node(_back);
		_back = prev;
	}
	void push_front(const T& value) {
		if (_size) {
			_head = create_node(nullptr, _head, value);
		} else {
			create_first_node(value);
		}
		_size++;
	}
	void push_front(T&& value) {
		if (_size) {
			_head = create_node(nullptr, _head, std::move(value));
		} else {
			create_first_node(std::move(value));
		}
		_size++;
	}
	template< class... Args >
	reference emplace_front(Args&&... args) {
		if (_size) {
			_head = create_node(nullptr, _head, std::forward<Args>(args)...);
		} else {
			create_first_node(std::forward<Args>(args)...);
		}
		_size++;
	}
	void pop_front() {
		_size--;
		node*next = _head->next;
		destory_node(_head);
		_head = next;
	}
	iterator erase(const_iterator pos) {
		
	}
	void swap(list& other) noexcept {
		size_type tmp_size = _size;
		_size = other._size;
		other._size = tmp_size;

		node* tmp_node = _head;
		_head = other._head;
		other._head = tmp_node;

		tmp_node = _back;
		_back = other._back;
		other._back = tmp_node;
	}


};

#endif // !LIST_HPP