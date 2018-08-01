#ifndef WEAK_PRT_HPP
#define WEAK_PRT_HPP


#include"detail/ref_count.hpp"
#include<type_traits>
#include"shared_ptr.hpp"

namespace LightSTL {


template<typename T>
class weak_ptr {
	template<typename Y>
	friend class shared_ptr;

	template<typename Y>
	friend class weak_ptr;
public:
	using element_type = std::remove_extent_t<T>;

	constexpr weak_ptr() noexcept 
		: ptr(nullptr), ref(nullptr) {}

	weak_ptr(const weak_ptr& r) noexcept
		: ptr(r.ptr), ref(r.ref) {
		if(ref)
			ref->add_weak_ptr();
	}

	template< class Y >
	weak_ptr(const shared_ptr<Y>& r) noexcept
		: ptr(r.ptr), ref(r.ref) {
		if(ref)
			ref->add_weak_ptr();
	}

	weak_ptr(weak_ptr&& r) noexcept
		: ptr(r.ptr), ref(r.ref) {
		r.ptr = nullptr;
		r.ref = nullptr;
	}

	template< class Y >
	weak_ptr(weak_ptr<Y>&& r) noexcept
		: ptr(r.ptr), ref(r.ref) {
		r.ptr = nullptr;
		r.ref = nullptr;
	}

	~weak_ptr() {
		FREE();
	}

	weak_ptr& operator=(const weak_ptr& r) noexcept {
		if (this == &r)
			return *this;
		FREE();
		ref = r.ref;
		ptr = r.ptr;
		if(ref)
			ref->add_weak_ptr();
		return *this;
	}

	template< class Y >
	weak_ptr& operator=(const weak_ptr<Y>& r) noexcept {
		if (this == &r)
			return *this;
		FREE();
		ref = r.ref;
		ptr = r.ptr;
		if(ref)
			ref->add_weak_ptr();
		return *this;
	}

	template< class Y >
	weak_ptr& operator=(const shared_ptr<Y>& r) noexcept {
		FREE();
		ref = r.ref;
		ptr = r.ptr;
		if(ref)
			ref->add_weak_ptr();
		return *this;
	}

	weak_ptr& operator=(weak_ptr&& r) noexcept {
		if (this == &r)
			return *this;
		FREE();
		ref = r.ref;
		ptr = r.ptr;
		r.ref = nullptr;
		r.ptr = nullptr;
		return *this;
	}

	template< class Y >
	weak_ptr& operator=(weak_ptr<Y>&& r) noexcept {
		if (this == &r)
			return *this;
		FREE();
		ref = r.ref;
		ptr = r.ptr;
		r.ref = nullptr;
		r.ptr = nullptr;
		return *this;
	}

	void reset() {
		FREE();
		ptr = nullptr;
		ref = nullptr;
	}

	void swap(weak_ptr& r)noexcept {
		element_type* tmp_ptr = r.ptr;
		detail::ref_count_base* tmp_ref = r.ref;
		r.ptr = ptr;
		r.ref = ref;
		ptr = tmp_ptr;
		ref = tmp_ref;
	}

	long use_count() const noexcept {
		if (ref)
			return ref->get_shared_cnt();
		else
			return 0;
	}

	bool expired() const noexcept {
		return use_count() == 0;
	}

	shared_ptr<T> lock() const noexcept {
		return shared_ptr<T>(*this);
	}

	template< class Y >
	bool owner_before(const weak_ptr<Y>& other) const noexcept {
		return ref < other->ref;
	}

	template< class Y >
	bool owner_before(const shared_ptr<Y>& other) const noexcept {
		return ref < other->ref;
	}
private:
	element_type* ptr;
	detail::ref_count_base* ref;
	void FREE() {
		if (ref) {
			ref->sub_weak_ptr();
			if (ref->get_weak_cnt() == 0) {
				ref->destory();
			}
		}
	}
};

template< class T >
void swap(weak_ptr<T>& lhs, weak_ptr<T>& rhs) noexcept {
	lhs.swap(rhs);
}

}
#endif // !WEAK_PRT_HPP
