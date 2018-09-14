#ifndef FORWARD_LIST_HPP
#define FORWARD_LIST_HPP

#include<cstddef>
#include<utility>
#include"memory/allocator.hpp"
#include"memory/addressof.hpp"
#include<initializer_list>
#include"iterator/iterator.hpp"


namespace detail {


	template<class T>
	class forward_list_node {
	public:

		T data;
		forward_list_node* next;

		template<class... Args>
		forward_list_node(forward_list_node* n, Args&&... args):data(std::forward<Args>(args)...),next(n) {}

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
	class forward_list_iterator {

		template<class Y>
		friend class forward_list_iterator;

		template<class T>
		friend typename forward_list_iterator<T>::list_node* get_node(forward_list_iterator<T> it);
	public:
		//typedefs
		using iterator_category = LightSTL::forward_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T * ;
		using reference = T & ;
	private:

		using element_type = typename remove_const<T>::value;
		using list_node = forward_list_node<element_type>;

		list_node * n;

	public:

		forward_list_iterator()
			: n(nullptr)  {}

		forward_list_iterator(list_node* x)
			: n(x) {}

		template<class Y>
		forward_list_iterator(const forward_list_iterator<Y>& other)
			: n(other.n) {}

		T& operator*()const { return n->data; }
		T* operator->()const { return LightSTL::addressof(n->data); }

		forward_list_iterator& operator++() {
			n = n->next;
			return *this;
		}

		forward_list_iterator operator++(int) {
			forward_list_iterator res = *this;
			++res;
			return res;
		}

		
		bool operator==(const forward_list_iterator& other) { return n == other.n; }
		bool operator!=(const forward_list_iterator& other) { return n != other.n; }
	};

	template<class T>
	friend typename forward_list_iterator<T>::list_node* get_node(forward_list_iterator<T> it) {
		return it.n;
	}

}


template<class T,class Allocator=LightSTL::allocator<T>>
class forward_list {

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

	using node = detail::forward_list_node<T>;

	node* head;
	Allocator::rebind<node>::other data_alloc;

	template<class... Args>
	node* create_new_node(Args&&... args)[
		
	]
};


#endif // !FORWARD_LIST_HPP
