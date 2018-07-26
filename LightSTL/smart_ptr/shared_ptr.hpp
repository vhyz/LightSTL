#ifndef SHARED_PTR_HPP
#define SHARED_PTR_HPP
#include"detail/ref_count.hpp"

template<typename T>class weak_ptr;
template<typename T,typename Deleter>class unique_ptr;

template<typename T>
class shared_ptr {
	template<typename Y> friend class shared_ptr;
	template<typename Y> friend class weak_ptr;
public:

	constexpr shared_ptr() noexcept {}

	constexpr shared_ptr(nullptr_t) noexcept {}

	template<typename Y>
	explicit shared_ptr(Y* p)
		: ptr(p), ref(new ref_block<Y>(p)) {
	}

	template<typename Y, typename Deleter>
	shared_ptr(Y* p, Deleter d)
		: ptr(p), ref(new ref_block<Y>(p, d)) {
	}

	template<typename Y>
	shared_ptr(const shared_ptr<Y>& r)noexcept
		: ptr(r.ptr), ref(r.ref) {
		ref->add_shared_ptr();
	}

	shared_ptr(const shared_ptr&r)noexcept
		: ptr(r.ptr), ref(r.ref) {
		ref->add_shared_ptr();
	}

	template<typename Y>
	shared_ptr(const shared_ptr<Y>&r, T* p)noexcept
		: ptr(p), ref(r.ref) {
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
		: ptr(static_cast<T*>(r.ptr)), ref(r.ref) {
	}

	template<typename T, typename Deleter>
	shared_ptr(unique_ptr<T, Deleter>&& r)
		: shared_ptr(r.get(), r.get_deleter()) {
		r.reset();
	}

	~shared_ptr() {
		FREE();
	}

	shared_ptr& operator=(const shared_ptr& r)noexcept {
		if (&r == this)
			return *this;
		FREE();
		ptr = r.ptr;
		ref = r.ref;
		r.ref->add_shared_ptr();
		return *this;
	}

	template<typename Y>
	shared_ptr& operator=(const shared_ptr<Y>& r)noexcept {
		if (&r == this)
			return *this;
		FREE();
		ptr = static_cast<T*>(r.ptr);
		ref = r.ref;
		r.ref->add_shared_ptr();
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
		ptr = static_cast<T*>(r.get());
		ref = new ref_block<Y, Deleter>(ptr, r.get_deleter());
		r.release();
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
		ref = new ref_block<Y>(p);
	}

	template<typename Y, typename Deleter>
	void reset(Y* p, Deleter d) {
		FREE();
		ptr = p;
		ref = new ref_block<Y, Deleter>(p, d);
	}

	void swap(shared_ptr& r) {

		T* p = r.ptr;
		r.ptr = ptr;
		ptr = tmp;

		ref_block_base* t = r.ref;
		r.ref = ref;
		ref = t;
	}

	T& operator*()const noexcept {
		return *ptr;
	}

	T* operator->()const noexcept {
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

	T* get()const noexcept {
		return ptr;
	}

private:
	T * ptr;
	ref_block_base* ref;
	void FREE() {
		if (ref) {
			ref->sub_shared_ptr();
			if (ref->get_shared_cnt() == 0) {
				ref->release();
				ref->sub_shared_ptr();
			}
			if (ref->get_weak_cnt() == 0) {
				delete ref;
			}
		}
	}
};
#endif // !SHARED_PTR_HPP
