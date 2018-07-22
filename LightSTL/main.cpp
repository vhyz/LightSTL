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

};

int main() {
	{
		unique_ptr<A>pa;
		{
			unique_ptr<A>pb(new A());
			pa = std::move(pb);
			std::cout << "Leave\n";
		}
		unique_ptr<A>pb(new B());
		pa = std::move(pb);
		std::vector<int>();
	}
	B*b;
	//A*a = b;
	
	system("pause");
}