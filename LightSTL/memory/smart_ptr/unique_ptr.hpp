#ifndef UNIQUE_PTR_HPP
#define UNIQUE_PTR_HPP
#include"detail/deleter.hpp"

namespace LightSTL {

template<typename T, typename Deleter = default_delete<T>>
class unique_ptr {
public:
	using pointer = T * ;
	using element_type = T;
	using deleter_type = Deleter;

	constexpr unique_ptr() noexcept
		:ptr(nullptr) {
	}

	constexpr unique_ptr(nullptr_t) noexcept
		:ptr(nullptr) {
	}

	explicit unique_ptr(T* p) noexcept
		:ptr(p) {
	}

	unique_ptr(T* p, const Deleter& d)noexcept
		:ptr(p), deleter(d) {
	}

	unique_ptr(T* p, Deleter&& d) noexcept
		:ptr(p), deleter(d) {
	}

	template<typename U>
	unique_ptr(unique_ptr<U>&& u)noexcept
		:ptr(u.release()), deleter(u.get_deleter()) {
		u.ptr = nullptr;
	}

	unique_ptr(const unique_ptr&) = delete;

	template<typename U>
	unique_ptr& operator =(unique_ptr<U>&& u)noexcept {
		if (&u == this)
			return *this;
		FREE();
		deleter = u.get_deleter();
		ptr = u.release();
	}

	unique_ptr& operator =(const unique_ptr&) = delete;

	unique_ptr& operator =(nullptr_t)noexcept {
		FREE();
		ptr = nullptr;
	}

	~unique_ptr() {
		FREE();
	}

	T* release()noexcept {
		T* res = ptr;
		ptr = nullptr;
		return res;
	}

	void reset(T* p = nullptr) {
		FREE();
		ptr = p;
	}

	void swap(unique_ptr& u) {
		T* tmp_ptr = u.ptr;
		u.ptr = ptr;
		ptr = tmp_ptr;

		Deleter tmp_deleter = u.deleter;
		u.deleter = deleter;
		deleter = tmp_deleter;
	}

	T* get()const noexcept {
		return ptr;
	}

	Deleter& get_deleter()noexcept {
		return deleter;
	}

	const Deleter& get_deleter()const noexcept {
		return deleter;
	}

	explicit operator bool() const noexcept {
		return this->get() != nullptr;
	}

	typename std::add_lvalue_reference<T>::type operator*() const {
		return *get(); //if ptr is nullptr,it is ub
	}

	T* operator->() const noexcept {
		return get();
	}
private:
	T * ptr;
	Deleter deleter;
	void FREE() {
		if (ptr != nullptr)
			deleter(ptr);
	}
};

template<typename T, typename Deleter>
class unique_ptr<T[], Deleter> {
public:
	constexpr unique_ptr() noexcept
		:ptr(nullptr) {
	}

	constexpr unique_ptr(nullptr_t) noexcept
		:ptr(nullptr) {
	}

	explicit unique_ptr(T* p)noexcept
		:ptr(p) {
	}

	unique_ptr(T* p, const Deleter& d)noexcept
		:ptr(p), deleter(d) {
	}

	unique_ptr(T* p, Deleter& d)noexcept
		:ptr(p), deleter(d) {
	}

	unique_ptr(const unique_ptr&) = delete;

	template<typename U>
	unique_ptr(unique_ptr<U[]>&& u)noexcept
		:ptr(u.ptr), deleter(u.deleter) {
	}

	unique_ptr& operator =(const unique_ptr&) = delete;

	template<typename U>
	unique_ptr<T>& operator =(unique_ptr<U[]>&& u)noexcept {
		if (&u == this)
			return;
		FREE();
		ptr = u.ptr;
		deleter = u.deleter;
		u.ptr = nullptr;
		return *this;
	}

	unique_ptr& operator =(nullptr_t)noexcept {
		FREE();
		ptr = nullptr;
	}

	~unique_ptr() {
		FREE();
	}

	T* release()noexcept {
		T* res = ptr;
		ptr = nullptr;
		return res;
	}

	void reset(T* p = nullptr) {
		FREE();
		ptr = p;
	}

	void swap(unique_ptr& u) {
		T* tmp_ptr = u.ptr;
		u.ptr = ptr;
		ptr = tmp_ptr;

		Deleter tmp_deleter = u.deleter;
		u.deleter = deleter;
		deleter = tmp_deleter;
	}

	T* get()const noexcept {
		return ptr;
	}

	Deleter& get_deleter()noexcept {
		return deleter;
	}

	const Deleter& get_deleter()const noexcept {
		return deleter;
	}

	explicit operator bool() const noexcept {
		return this->get() != nullptr;
	}

	T& operator[] (size_t i)const {
		return get()[i];
	}
private:
	T * ptr;
	Deleter deleter;
	void FREE() {
		if (ptr != nullptr)
			deleter(ptr);
	}
};

template<class T1, class D1, class T2, class D2>
bool operator==(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y) {
	return x.get() == y.get();
}

template<class T1, class D1, class T2, class D2>
bool operator!=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y) {
	return x.get() != y.get();
}

template <class T, class D>
bool operator==(const unique_ptr<T, D>& x, nullptr_t) noexcept {
	return !x;
}

template <class T, class D>
bool operator==(nullptr_t, const unique_ptr<T, D>& x) noexcept {
	return !x;
}

template <class T, class D>
bool operator!=(const unique_ptr<T, D>& x, nullptr_t) noexcept {
	return static_cast<bool>(x);
}

template <class T, class D>
bool operator!=(nullptr_t, const unique_ptr<T, D>& x) noexcept {
	return static_cast<bool>(x);
}

template< class T, class Deleter >
void swap(unique_ptr<T, Deleter>& lhs, unique_ptr<T, Deleter>& rhs) noexcept {
	lhs.swap(rhs);
}

}
#endif // !UNIQUE_PTR_HPP