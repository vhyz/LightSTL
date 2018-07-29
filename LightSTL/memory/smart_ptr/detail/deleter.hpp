#ifndef DELETER_HPP
#define DELETER_HPP

namespace LightSTL {


template<typename T>
struct default_delete {
	constexpr default_delete() noexcept = default;

	template <typename U>
	default_delete(const default_delete<U>& d) noexcept {}

	template<typename U>
	default_delete(default_delete&& u)noexcept {}

	template<typename U>
	default_delete& operator = (const default_delete<U>& u)noexcept {}

	void operator()  (T* ptr) {
		delete ptr;
	}

	~default_delete() = default;

};

template<typename T>
struct default_delete<T[]> {
	constexpr default_delete() noexcept = default;

	template<class U>
	default_delete(const default_delete<U[]>& d) noexcept {}

	template<typename U>
	default_delete(default_delete<U[]>&& u)noexcept {}

	template<typename U>
	default_delete operator = (const default_delete<U[]>& u)noexcept {}

	void operator() (T* ptr) {
		delete[] ptr;
	}

	~default_delete() = default;
};

}
#endif // !DELETER_HPP
