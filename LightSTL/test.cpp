#include<vector>
#include<queue>
#include<memory>
#include"allocator.h"
#include<iostream>
#include<ctime>
#include"smart_ptr/shared_ptr.hpp"
#include"smart_ptr/unique_ptr.hpp"

using std::cout;
using std::endl;

class A {
public:
	~A() {
		cout << "~A";
	}
};

int main() {
	{
		unique_ptr<A[]> a(new A[3]());
		shared_ptr<A>b(new A());
		shared_ptr<A>c(b);
		shared_ptr<A>d(b);
		cout << b.use_count();
	}
	system("pause");
}
