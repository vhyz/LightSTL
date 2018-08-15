#ifndef SHARED_PTR_HPP
#define SHARED_PTR_HPP

//标准规定的多线程原子操作函数暂时没有写，而C++20中 std::atomic<std::shared_ptr>代替了这些操作

#include"detail/ref_count.hpp"
#include<type_traits>
#include<iostream>

namespace LightSTL {


template<typename T>class weak_ptr;
template<typename T,typename Deleter>class unique_ptr;
template<typename T>class enable_shared_from_this;

template<typename T>
class shared_ptr {
	template<typename Y> 
	friend class shared_ptr;

	template<typename Y> 
	friend class weak_ptr;

	template<typename Deleter, typename T>
	friend Deleter* get_deleter(const shared_ptr<T>& p) noexcept;

	template<typename Y, typename... Args>
	friend shared_ptr<Y> make_shared(Args&&... args);
public:
	using element_type = std::remove_extent_t<T>;
	using weak_type = weak_ptr<T>;

	constexpr shared_ptr() noexcept
		: ptr(nullptr), ref(nullptr) {}

	constexpr shared_ptr(nullptr_t) noexcept 
		: ptr(nullptr), ref(nullptr) {}

	template<typename Y>
	explicit shared_ptr(Y* p)
		: ptr(p), ref(new detail::ref_count<std::conditional_t<std::is_array_v<T>, Y[], Y>>(p)) {
		_shared_from_this(p);
	}

	template<typename Y, typename Deleter>
	shared_ptr(Y* p, Deleter d)
		: ptr(p), ref(new detail::ref_count<std::conditional_t<std::is_array_v<T>, Y[], Y>>(p, d)) {
		_shared_from_this(p);
	}

	template<typename Y>
	shared_ptr(const shared_ptr<Y>& r)noexcept
		: ptr(r.ptr), ref(r.ref) {
		if(ref)
			ref->add_shared_ptr();
	}

	shared_ptr(const shared_ptr&r)noexcept
		: ptr(r.ptr), ref(r.ref) {
		if(ref)
			ref->add_shared_ptr();
	}

	template<typename Y>
	shared_ptr(const shared_ptr<Y>&r, element_type* p)noexcept
		: ptr(p), ref(r.ref) {
		if(ref)
			ref->add_shared_ptr();
	}

	shared_ptr(shared_ptr&& r)noexcept
		: ptr(r.ptr), ref(r.ref) {
		r.ptr = nullptr;
		r.ref = nullptr;
	}

	template<typename Y>
	shared_ptr(shared_ptr<Y>&& r)noexcept
		: ptr(r.ptr), ref(r.ref) {
		r.ptr = nullptr;
		r.ref = nullptr;
	}

	template<typename Y>
	explicit shared_ptr(const weak_ptr<Y>& r)
		: ptr(r.ptr), ref(r.ref) {
		if (ref)
			ref->add_shared_ptr();
	}

	template<typename T, typename Deleter>
	shared_ptr(unique_ptr<T, Deleter>&& r)
		: shared_ptr(r.release(), r.get_deleter()) {}

	~shared_ptr() {
		FREE();
	}

	shared_ptr& operator=(const shared_ptr& r)noexcept {
		if (&r == this)
			return *this;
		FREE();
		ptr = r.ptr;
		ref = r.ref;
		if(ref)
			ref->add_shared_ptr();
		return *this;
	}

	template<typename Y>
	shared_ptr& operator=(const shared_ptr<Y>& r)noexcept {
		if (&r == this)
			return *this;
		FREE();
		ptr = r.ptr;
		ref = r.ref;
		if(ref)
			rref->add_shared_ptr();
		return *this;
	}

	shared_ptr& operator=(shared_ptr&& r)noexcept {
		if (&r = this)
			return *this;
		FREE();
		ptr = r.ptr;
		ref = r.ref;
		return *this;
	}

	template<typename Y>
	shared_ptr& operator=(shared_ptr<Y>&& r)noexcept {
		if (&r == this)
			return *this;
		FREE();
		ptr = r.ptr;
		ref = r.ref;
		return *this;
	}

	template<typename Y, typename Deleter>
	shared_ptr& operator=(unique_ptr<Y, Deleter>&& r) {
		FREE();
		ptr = static_cast<T*>(r.release());
		ref = new detail::ref_count<Y, Deleter>(ptr, r.get_deleter());
		return *this;
	}

	void reset()noexcept {
		FREE();
		ptr = nullptr;
		ref = nullptr;
	}

	template<typename Y>
	void reset(Y* p) {
		FREE();
		ptr = static_cast<T*>(p);
		ref = new detail::ref_count<std::conditional_t<std::is_array_v<T>, Y[], Y>>(p);
	}

	template<typename Y, typename Deleter>
	void reset(Y* p, Deleter d) {
		FREE();
		ptr = p;
		ref = new detail::ref_count<std::conditional_t<std::is_array_v<T>, Y[], Y>>(p, d);
	}

	void swap(shared_ptr& r) {
		element_type* p = r.ptr;
		r.ptr = ptr;
		ptr = p;

		ref_count_base* t = r.ref;
		r.ref = ref;
		ref = t;
	}

	element_type& operator*()const noexcept {
		return *ptr;
	}

	element_type* operator->()const noexcept {
		return ptr;
	}

	long use_count() const noexcept {
		if (ref == nullptr)
			return 0;
		return ref->get_shared_cnt();
	}

	bool unique() const noexcept {
		return use_count() == 1;
	}

	explicit operator bool() const noexcept {
		return get() != nullptr;
	}

	template< class Y >
	bool owner_before(const shared_ptr<Y>& other) const noexcept {
		return ref < other.ref;
	}

	template< class Y >
	bool owner_before(const std::weak_ptr<Y>& other) const noexcept {
		return ref < other.ref;
	}

	element_type* get()const noexcept {
		return ptr;
	}

private:
	element_type * ptr;
	detail::ref_count_base* ref;

	void FREE() {
		if (ref) {
			ref->sub_shared_ptr();
			if (ref->get_shared_cnt() == 0) {
				ref->release();
				ref->sub_weak_ptr();
			}
			if (ref->get_weak_cnt() == 0) {
				ref->destory();
			}
		}
	}

	template<typename Y, std::enable_if_t<!std::is_base_of_v<enable_shared_from_this<Y>, Y>, int> = 0>
	void _shared_from_this(Y* p) {}

	template<typename Y, std::enable_if_t<std::is_base_of_v<enable_shared_from_this<Y>,Y>,int> = 0>
	void _shared_from_this(Y* p) {
		p->_internal_accept_owner(*this);
	}


};

template<typename T, typename... Args>  
shared_ptr<T> make_shared(Args&&... args) {
	//T不是数组类型
	
	//仅进行一次分配

	detail::ms_ref_count<T>* ms_count = new detail::ms_ref_count<T>(std::forward<Args>(args)...);

	shared_ptr<T>res;
	res.ptr = ms_count->ptr;
	res.ref = ms_count;

	return res;
}

template<typename Deleter, typename T>
Deleter* get_deleter(const shared_ptr<T>& p) noexcept {
	if (p->ref == nullptr)
		return nullptr;
	else
		return (Deleter*)p->ref->get_delete();
}

template< class T >
void swap(shared_ptr<T>&lhs, shared_ptr<T>&rhs)noexcept {
	lhs.swap(rhs);
}

template < class T, class U >
bool operator==(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) noexcept {
	return lhs.get() == rhs.get();
}

template< class T, class U >
bool operator!=(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) noexcept {
	return lhs.get() != rhs.get();
}

template< class T, class U >
bool operator<(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) noexcept {
	return lhs.get() < rhs.get();
}

template< class T, class U >
bool operator>(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) noexcept {
	return rhs > lhs;
}

template< class T, class U >
bool operator<=(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) noexcept {
	return !(lhs > rhs);
}

template< class T, class U >
bool operator>=(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) noexcept {
	return !(lhs < rhs);
}
template< class T >
bool operator==(const shared_ptr<T>& lhs, std::nullptr_t rhs) noexcept {
	return !lhs;
}

template< class T >
bool operator==(std::nullptr_t lhs, const shared_ptr<T>& rhs) noexcept {
	return !rhs;
}

template< class T >
bool operator!=(const shared_ptr<T>& lhs, std::nullptr_t rhs) noexcept {
	return (bool)lhs;
}

template< class T >
bool operator!=(std::nullptr_t lhs, const shared_ptr<T>& rhs) noexcept {
	return (bool)rhs;
}

template< class T >
bool operator<(const shared_ptr<T>& lhs, std::nullptr_t rhs) noexcept {
	return lhs.get() < nullptr;
}

template< class T >
bool operator<(std::nullptr_t lhs, const shared_ptr<T>& rhs) noexcept {
	return nullptr < rhs.get();
}

template< class T >
bool operator>(const shared_ptr<T>& lhs, std::nullptr_t rhs) noexcept {
	return lhs.get() > rhs;
}

template< class T >
bool operator>(std::nullptr_t lhs, const shared_ptr<T>& rhs) noexcept {
	return lhs > rhs.get();
}

template< class T >
bool operator<=(const shared_ptr<T>& lhs, std::nullptr_t rhs) noexcept {
	return !(lhs.get(0 > rhs));
}

template< class T >
bool operator<=(std::nullptr_t lhs, const shared_ptr<T>& rhs) noexcept {
	return !(lhs > rhs.get());
}

template< class T >
bool operator>=(const shared_ptr<T>& lhs, std::nullptr_t rhs) noexcept {
	return !(lhs.get() < rhs);
}

template< class T >
bool operator>=(std::nullptr_t lhs, const shared_ptr<T>& rhs) noexcept {
	return !(lhs < rhs.get());
}
template <class T, class U, class V>
std::basic_ostream<U, V>& operator<<(std::basic_ostream<U, V>& os, const std::shared_ptr<T>& ptr) {
	os << ptr.get();
	return os;
}


}

#endif // !SHARED_PTR_HPP
