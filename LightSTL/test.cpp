#include<vector>
#include<queue>
#include<memory>
#include"allocator.h"
#include<iostream>
#include<ctime>
#include"smart_ptr/shared_ptr.hpp"
#include"smart_ptr/unique_ptr.hpp"
#include"smart_ptr/weak_ptr.hpp"
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

int main() {
	
	{
		weak_ptr<A>wp;
		{
			std::cout << "enter block\n";
			shared_ptr<A[]>a(new A[3]());
			shared_ptr<A>b = make_shared<A>();
			wp = b;
		}
		std::cout << "leave block\n";
	}

	system("pause");
}
