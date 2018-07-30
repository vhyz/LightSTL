#ifndef UNINITIALIZED_STORAGE_HPP
#define UNINITIALIZED_STORAGE_HPP

#include"../iterator/iterator_traits.hpp"
#include"addressof.hpp"

namespace LightSTL {



template<class InputIt, class ForwardIt>
ForwardIt uninitialized_copy(InputIt first, InputIt last, ForwardIt d_first)
{
    typedef typename std::iterator_traits<ForwardIt>::value_type Value;
    ForwardIt current = d_first;
    try {
        for (; first != last; ++first, (void) ++current) {
            ::new (static_cast<void*>(std::addressof(*current))) Value(*first);
        }
        return current;
    } catch (...) {
        for (; d_first != current; ++d_first) {
            d_first->~Value();
        }
        throw;
    }
}


template< class InputIt, class Size, class ForwardIt >
ForwardIt uninitialized_copy_n(InputIt first, Size count, ForwardIt d_first) {
	typedef typename iterator_traits<ForwardIt>::value_type Value;
	ForwardIt current = d_first;
	try {
		for (; first != last; ++first, (void) ++current) {
			::new (static_cast<void*>(addressof(*current))) Value(*first);
		}
		return current;
	} catch (...) {
		for (; d_first != current; ++d_first) {
			d_first->~Value();
		}
		throw;
	}
}


template<class ForwardIt, class T>
void uninitialized_fill(ForwardIt first, ForwardIt last, const T& value) {
	typedef typename std::iterator_traits<ForwardIt>::value_type Value;
	ForwardIt current = first;
	try {
		for (; current != last; ++current) {
			::new (static_cast<void*>(std::addressof(*current))) Value(value);
		}
	} catch (...) {
		for (; first != current; ++first) {
			first->~Value();
		}
		throw;
	}
}

template< class ForwardIt, class Size, class T >
ForwardIt uninitialized_fill_n(ForwardIt first, Size count, const T& value) {
	typedef typename std::iterator_traits<ForwardIt>::value_type Value;
	ForwardIt current = first;
	try {
		for (; count > 0; ++current, (void) --count) {
			::new (static_cast<void*>(std::addressof(*current))) Value(value);
		}
		return current;
	} catch (...) {
		for (; first != current; ++first) {
			first->~Value();
		}
		throw;
	}
}




}
#endif // !UNINITIALIZED_STORAGE_HPP
