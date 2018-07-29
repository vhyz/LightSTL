#ifndef ENABLE_SHARED_FROM_THIS_HPP
#define ENABLE_SHARED_FROM_THIS_HPP
#include"shared_ptr.hpp"
#include"weak_ptr.hpp"
namespace LightSTL{

template<typename T>
class enable_shared_from_this {
protected:
	constexpr enable_shared_from_this()noexcept {}

	enable_shared_from_this(const enable_shared_from_this<T>&obj) noexcept {}

	virtual ~enable_shared_from_this()noexcept {}

	enable_shared_from_this<T>& operator=(const enable_shared_from_this<T> &obj) noexcept {
		return *this;
	}

public:
	shared_ptr<T> shared_from_this() {
		return shared_ptr<T>(weak_this_);
	}

	shared_ptr<T const> shared_from_this() const {
		return shared_ptr<T const>(weak_this_);
	}

	weak_ptr<T> weak_from_this() noexcept {
		return weak_this_;
	}

	weak_ptr<T const> weak_from_this() const noexcept {
		return weak_this_;
	}


	//仅由shared_ptr调用
	template<typename X> void _internal_accept_owner(const shared_ptr<X>& sp) const {
		if (weak_this_.expired()) {
			weak_this_ = sp;
		}
	}

private:
	mutable weak_ptr<T> weak_this_;
};


}

#endif // !ENABLE_SHARED_FROM_THIS_HPP
