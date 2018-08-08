#ifndef REF_COUNT_HPP
#define REF_COUNT_HPP
#include<atomic>
#include"deleter.hpp"
#include<cstddef>
#include<iostream>

namespace LightSTL{


namespace detail {


class ref_count_base {
private:
	std::atomic<size_t>shared_cnt;
	std::atomic<size_t>weak_cnt;
public:
	ref_count_base()noexcept
		:shared_cnt(1), weak_cnt(1) {
	}

	inline void add_shared_ptr() {
		shared_cnt.fetch_add(1, std::memory_order_relaxed);
	}

	inline void sub_shared_ptr() {
		shared_cnt.fetch_sub(1, std::memory_order_seq_cst);
	}

	inline void add_weak_ptr() {
		weak_cnt.fetch_add(1, std::memory_order_relaxed);
	}

	inline void sub_weak_ptr() {
		weak_cnt.fetch_sub(1, std::memory_order_seq_cst);
	}

	inline size_t get_shared_cnt() {
		return shared_cnt.load();
	}

	inline size_t get_weak_cnt() {
		return weak_cnt.load();
	}

	virtual ~ref_count_base()noexcept {}
	virtual void* get_deleter() = 0;
	virtual void release() = 0;
	virtual void destory() = 0;
};

template<typename T, typename Deleter = default_delete<T>>
struct ref_count :public ref_count_base {
	T* ptr;
	Deleter deleter;

	ref_count()
		:ptr(nullptr), ref_count_base() {}

	ref_count(std::nullptr_t)
		:ptr(nullptr), ref_count_base() {}

	ref_count(T* p)
		:ptr(p), ref_count_base() {}

	ref_count(T* p, const Deleter& d)
		:ptr(p), deleter(d), ref_count_base() {}

	ref_count(const ref_count&) = delete;

	ref_count(ref_count&&) = delete;

	ref_count& operator=(const ref_count) = delete;

	virtual ~ref_count() noexcept {}

	virtual void* get_deleter()override {
		return &deleter;
	}

	virtual void release()override {
		deleter(ptr);
	}

	virtual void destory()override {
		delete this;
		std::cout << "deleter ref\n";
	}
};

template<typename T, typename Deleter>
struct ref_count<T[], Deleter> :public ref_count_base {
	T* ptr;
	Deleter deleter;

	ref_count()
		:ptr(nullptr), ref_count_base() {}

	ref_count(std::nullptr_t)
		:ptr(nullptr), ref_count_base() {}

	ref_count(T* p)
		:ptr(p), ref_count_base() {}

	ref_count(T* p, const Deleter& d)
		:ptr(p), deleter(d), ref_count_base() {}

	ref_count(const ref_count&) = delete;

	ref_count(ref_count&&) = delete;

	ref_count& operator=(const ref_count&) = delete;

	~ref_count() noexcept {}

	virtual void* get_deleter()override {
		return &deleter;
	}

	virtual void release()override {
		deleter(ptr);
	}

	virtual void destory() override {
		delete this;
		std::cout << "deleter ref\n";
	}
};

template<typename T>
struct ms_ref_count :public ref_count<T>  {
private:
	T data;
public:
	template<class... Args>
	ms_ref_count(Args&&... args)
		: data(std::forward<Args>(args)...) {
		ptr = &data;
	}

	virtual void release()override  {}

	virtual void destory() override {
		delete this;
		std::cout << "deleter ms_ref\n";
	}

};


}

}
#endif // !REF_COUNT_HPP
