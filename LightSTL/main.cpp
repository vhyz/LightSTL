#include<vector>
#include<queue>
#include<memory>
#include"allocator.h"
#include<iostream>
#include<ctime>
#include"ptr.h"
#include<set>

class A {
public:
	~A() {
		std::cout << "~A\n";
	}
};

class B :public A {
public:
	~B() {
		std::cout << "~B\n";
	}
};


int main() {
	{
		std::shared_ptr<B>a(new B());
		std::shared_ptr<A>b = a;
	}
	std::
	system("pause");
}