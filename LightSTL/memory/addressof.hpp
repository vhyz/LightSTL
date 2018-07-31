#ifndef ADDRESSOF_HPP
#define ADDRESSOF_HPP


//下面的实现为boost中的简化实现
namespace LightSTL {


template< class T >
T* addressof(T& arg) 
{
    return reinterpret_cast<T*>(
		&const_cast<char&>(
			reinterpret_cast<const volatile char&>(arg)));
}

}
#endif // !ADDRESSOF_HPP
