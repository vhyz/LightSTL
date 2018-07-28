#include<vector>
#include<queue>
#include<memory>
#include"allocator.h"
#include<iostream>
#include<ctime>
#include"smart_ptr/shared_ptr.hpp"
#include"smart_ptr/unique_ptr.hpp"
#include"smart_ptr/weak_ptr.hpp"
#include"smart_ptr/enable_shared_from_this.hpp"
#include<memory>
#include<string>

using std::cout;
using std::endl;
using std::string;
class A {
public:
	~A() {
		cout << "~A\n";
	}
};

class B :public LightSTL::enable_shared_from_this<B> {
public:
	~B() {
		cout << "~B\n";
	}
};

int main() {
	
	{
		LightSTL::weak_ptr<A>wp;
		{
			std::cout << "enter block\n";
			LightSTL::shared_ptr<A[]>a(new A[3]());
			LightSTL::shared_ptr<A>b = LightSTL::make_shared<A>();
			wp = b;
		}
		std::cout << "leave block\n";
	}


	{
		LightSTL::shared_ptr<B>a(new B());
		LightSTL::shared_ptr<B>b = a->shared_from_this();
		cout << a.use_count();
	}
	system("pause");
}