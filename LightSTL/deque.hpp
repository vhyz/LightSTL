#ifndef DEQUE_HPP
#define DEQUE_HPP


#include"iterator/iterator.hpp"


namespace detail {

	inline size_t __deque_buf_size(size_t n, size_t sz) {
		return n != 0 ? n : (sz < 512 ? (512 / sz) : 1);
	}

	template<class T, size_t BufSiz>
	class deque_iterator {
	public:		
		using iterator_category = LightSTL::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T * ;
		using reference = T & ;
	private:
		using map_pointer = T * *;

		static size_t buf_size() {
			return __deque_buf_size(BufSiz, sizeof(T));
		}

		T* cur;
		T* first;
		T* last;
		map_pointer node;

		void set_node(map_pointer new_node) {
			node = new_node;
			first = *new_node;
			last = first + difference_type(buf_size());
		}
	public:
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


	};


}








#endif // !DEQUE_HPP
