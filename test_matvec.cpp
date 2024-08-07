/*
PASSED: test_matvec: result equal to file
--- 1/1 checks passed ---
************************************************************************************************
==2011== LEAK SUMMARY:
==2011==    definitely lost: 0 bytes in 0 blocks
==2011==    indirectly lost: 0 bytes in 0 blocks
==2011==      possibly lost: 0 bytes in 0 blocks
==2011==    still reachable: 145,816 bytes in 3 blocks
==2011==         suppressed: 0 bytes in 0 blocks
==2011==
==2011== For lists of detected and suppressed errors, rerun with: -s
==2011== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
*/

#include "matvec.hpp"
#include "tensor.hpp"

#include "gtest/gtest.h"



TEST(MatVecTest, test_matvec) {
  Matrix<int> A("data/matrix");
  Vector<int> x("data/vector_in");
  Vector<int> y_read("data/vector_out");

  std::cout << A.tensor() << std::endl;
  std::cout << x.tensor() << std::endl;
  std::cout << y_read.tensor() << std::endl;

  auto y_comp = matvec(A, x);

  EXPECT_EQ(y_comp.tensor(), y_read.tensor());
}

