#ifndef UNINITIALIZED_STORAGE_HPP
#define UNINITIALIZED_STORAGE_HPP

#include"../iterator/iterator_traits.hpp"
#include"addressof.hpp"
#include<utility>  //std::pair

namespace LightSTL {



template<class InputIt, class ForwardIt>
ForwardIt uninitialized_copy(InputIt first, InputIt last, ForwardIt d_first)
{
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
	typedef typename iterator_traits<ForwardIt>::value_type Value;
	ForwardIt current = first;
	try {
		for (; current != last; ++current) {
			::new (static_cast<void*>(addressof(*current))) Value(value);
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
	typedef typename iterator_traits<ForwardIt>::value_type Value;
	ForwardIt current = first;
	try {
		for (; count > 0; ++current, (void) --count) {
			::new (static_cast<void*>(LightSTL::addressof(*current))) Value(value);
		}
		return current;
	} catch (...) {
		for (; first != current; ++first) {
			first->~Value();
		}
		throw;
	}
}


template<class InputIt, class ForwardIt>
ForwardIt uninitialized_move(InputIt first, InputIt last, ForwardIt d_first) {
	typedef typename iterator_traits<ForwardIt>::value_type Value;
	ForwardIt current = d_first;
	try {
		for (; first != last; ++first, (void) ++current) {
			::new (static_cast<void*>(LightSTL::addressof(*current))) Value(std::move(*first));
		}
		return current;
	} catch (...) {
		for (; d_first != current; ++d_first) {
			d_first->~Value();
		}
		throw;
	}
}

template<class InputIt, class Size, class ForwardIt>
std::pair<InputIt, ForwardIt> uninitialized_move_n(InputIt first, Size count, ForwardIt d_first) {
	typedef typename iterator_traits<ForwardIt>::value_type Value;
	ForwardIt current = d_first;
	try {
		for (; count > 0; ++first, (void) ++current, --count) {
			::new (static_cast<void*>(addressof(*current))) Value(std::move(*first));
		}
	} catch (...) {
		for (; d_first != current; ++d_first) {
			d_first->~Value();
		}
		throw;
	}
	return { first, current };
}


template<class ForwardIt>
void uninitialized_default_construct(ForwardIt first, ForwardIt last) {
	using Value = typename std::iterator_traits<ForwardIt>::value_type;
	ForwardIt current = first;
	try {
		for (; current != last; ++current) {
			::new (static_cast<void*>(addressof(*current))) Value();
		}
	} catch (...) {
		for (; first != last; ++first)
			first->~Value();
		throw;
	}
}

template<class ForwardIt, class Size>
ForwardIt uninitialized_default_construct_n(ForwardIt first, Size n) {
	using T = typename std::iterator_traits<ForwardIt>::value_type;
	ForwardIt current = first;
	try {
		for (; n > 0; (void) ++current, --n) {
			::new (static_cast<void*>(addressof(*current))) T();
		}
		return current;
	} catch (...) {
		for (; first != current; ++first)
			first->~T();
		throw;
	}
}
template<class ForwardIt, class Size>
ForwardIt destroy_n(ForwardIt first, Size n) {
	for (; n > 0; (void) ++first, --n)
		destroy_at(std::addressof(*first));
	return first;
}
template< class ForwardIt >
void destroy(ForwardIt first, ForwardIt last) {
	for (; first != last; ++first)
		destroy_at(std::addressof(*first));
}
template<class T>
void destroy_at(T* p) {
	p->~T();
}

}
#endif // !UNINITIALIZED_STORAGE_HPP
