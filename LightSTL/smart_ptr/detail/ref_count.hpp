#ifndef REF_COUNT_HPP
#define REF_COUNT_HPP
#include<atomic>

class ref_block_base {
private:
	std::atomic<size_t>shared_cnt;
	std::atomic<size_t>weak_cnt;
public:
	ref_block_base()noexcept
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

	virtual ~ref_block_base()noexcept {}
	virtual void* get_deleter() = 0;
	virtual void release() = 0;
};

template<typename T, typename Deleter = default_delete<T>>
struct ref_block :public ref_block_base {
	T* ptr;
	Deleter deleter;

	ref_block()
		:ptr(nullptr), ref_block_base() {
	}

	ref_block(nullptr_t)
		:ptr(nullptr), ref_block_base() {
	}

	ref_block(T* p)
		:ptr(p), ref_block_base() {
	}

	ref_block(T* p, const Deleter& d)
		:ptr(p), deleter(d), ref_block_base() {
	}

	ref_block(const ref_block&) = delete;

	ref_block(ref_block&&) = delete;

	ref_block& operator=(const ref_block) = delete;

	~ref_block() noexcept {}

	void* get_deleter()override {
		return &deleter;
	}

	void release()override {
		deleter(ptr);
		ptr = nullptr;
	}
};

template<typename T, typename Deleter>
struct ref_block<T[], Deleter> :public ref_block_base {
	T* ptr;
	Deleter deleter;

	ref_block()
		:ptr(nullptr), ref_block_base() {
	}

	ref_block(nullptr_t)
		:ptr(nullptr), ref_block_base() {
	}

	ref_block(T* p)
		:ptr(p), ref_block_base() {
	}

	ref_block(T* p, const Deleter& d)
		:ptr(p), deleter(d), ref_block_base() {
	}

	ref_block(const ref_block&) = delete;

	ref_block(ref_block&&) = delete;

	ref_block& operator=(const ref_block&) = delete;

	~ref_block() noexcept {}

	inline void* get_deleter()override {
		return &deleter;
	}

	void release()override {
		deleter(ptr);
		ptr = nullptr;
	}
};

#endif // !REF_COUNT_HPP
