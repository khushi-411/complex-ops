#include <c10/tests/complex_test_common.h>

TEST(NonStaticTests, all) {
  run_all_host_tests();
}

int main() {
  NonStaticTests_all();
}
