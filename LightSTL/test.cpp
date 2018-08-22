#include"test/addressof_test.hpp"
#include"test/test_uninitialized_storage.hpp"
#include"test/test_vector.hpp"
#include"test/test_allocator.hpp"
#include"test/test_list.hpp"
#include<deque>
#include<memory>

int main() {
	addressof_test();
	test::test_uninitialized_storage();
	test::test_vector();
	TEST_allocator::test_allocator();
	TEST_LIST::test_list();
	system("pause");
	return 0;
}
