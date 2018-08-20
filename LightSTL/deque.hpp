#ifndef DEQUE_HPP
#define DEQUE_HPP


#include"iterator/iterator.hpp"
#include"memory/allocator.hpp"
#include"memory/allocator_traits.hpp"

namespace detail {

	template<class T>
	struct remove_const {
		using value = T;
	};

	template<class T>
	struct remove_const<const T> {
		using value = T;
	};


	inline size_t __deque_buf_size(size_t n, size_t sz) {
		return n != 0 ? n : (sz < 512 ? (512 / sz) : 1);
	}

	template<class T, size_t BufSiz = 0>
	class deque_iterator {

		template<class T,size_t BufSize = 0>
		friend class deque_iterator;
	public:		
		//iterator typedefs
		using iterator_category = LightSTL::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T * ;
		using reference = T & ;
	private:
		using element_type = typename remove_const<T>::value;
		using map_pointer = element_type * *;

		static size_t buf_size() {
			return __deque_buf_size(BufSiz, sizeof(T));
		}

		element_type* cur;
		element_type* first;
		element_type* last;
		map_pointer node;

		void set_node(map_pointer new_node) {
			node = new_node;
			first = *new_node;
			last = first + difference_type(buf_size());
		}
	public:

		deque_iterator()
			: cur(nullptr), first(nullptr), last(nullptr),node(nullptr) {}

		template<class Y>
		deque_iterator(const deque_iterator<Y,BufSiz>& other )
			: cur(other.cur), first(other.first), last(other.last), node(other.node)  {}

		template<class Y>
		deque_iterator& operator=(const deque_iterator<Y, BufSiz>& other) {
			cur = other.cur;
			first = other.first;
			last = other.last;
			node = other.node;
		}

		reference operator*()const { return *cur; }
		pointer operator->()const { return cur; }

		difference_type operator-(const deque_iterator& other)const {
			return (node - other.node)*buf_size() + (node - first) - (other.node - other.first);
		}

		deque_iterator& operator++() {
			++cur;
			if (cur == last) {
				set_node(node + 1);
				cur = first;
			}
			return *this;
		}

		deque_iterator operator++(int) {
			deque_iterator res = *this;
			++*this;
			return res;
		}

		deque_iterator& operator--() {
			if (cur == first) {
				set_node(node - 1);
				cur = last;
			}
			--cur;
			return *this;
		}

		deque_iterator operator--(int) {
			deque_iterator res = *this;
			--*this;
			return res;
		}

		deque_iterator& operator+=(difference_type n) {
			difference_type offset = n + cur - first;
			if (offset >= 0 && offset < difference_type(buf_size())) {
				
				//若相加后处于同一缓冲区
				cur += n;
			} else {

				//不在同一缓冲区
				difference_type new_node_offset = offset >= 0 ? offset / difference_type(buf_size())
					: -(-offset - 1) / difference_type(buf_size()) - 1;

				set_node(node + new_node_offset);

				cur = first + (offset - new_node_offset * difference_type(buf_size()));
			}
			return *this;
		}

		deque_iterator operator+(difference_type n) {
			deque_iterator res = *this;
			res += n;
			return res;
		}

		deque_iterator& operator-=(difference_type n) {
			return *this += -n;
		}

		deque_iterator operator-(difference_type n) {
			deque_iterator res = *this;
			res -= n;
			return res;
		}

		reference operator[](difference_type n) { return *(*this += n); }

		bool operator==(const deque_iterator& other) { return cur == other.cur; }
		bool operator!=(const deque_iterator& other) { return cur != other.cur; }
		bool operator<(const deque_iterator& other)  { return cur < other.cur; }
	};

}



template<class T,class Allocator = LightSTL::allocator<T> >
class deque {
public:

	//typedefs
	using value_type = T;
	using allocator_type = Allocator;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = value_type & ;
	using const_reference = const value_type&;
	using pointer = T * ;
	using const_pointer = const T*;
	using iterator = detail::deque_iterator<T>;
	using const_iterator = detail::deque_iterator<const T>;
	using reverse_iterator = LightSTL::reverse_iterator<iterator>;
	using const_reverse_iterator = LightSTL::reverse_iterator<const_iterator>;

private:

	using map_pointer = T * *;
	//数据成员
	iterator start;
	iterator finish;
	//缓冲区管控
	map_pointer map;
	size_type map_size;

public:

	deque()
		: start(), finish(), map(nullptr), map_size(0) {}


	explicit deque(const Allocator& alloc);
};








#endif // !DEQUE_HPP
