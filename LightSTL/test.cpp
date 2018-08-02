#include"test/addressof_test.hpp"
#include"test/test_uninitialized_storage.hpp"
#include"test/test_vector.hpp"

int main() {
	addressof_test();
	//test::test_uninitialized_storage();
	test::test_vector();
	system("pause");
	return 0;
}
