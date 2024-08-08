
#include "gtest/gtest.h"
#include "tensor.hpp"

TEST(TensorTest, Constructor)
{
    Tensor< int> a;
    Tensor<int> b({3, 5, 8});
    Tensor<int> c({3, 5, 8}, 42);
    Tensor<int> d({3, 5, 8, 6, 2, 8, 2, 7, 7, 8}, 42);

    ASSERT_EQ(c.numElements(), 3 * 5 * 8) << "failure in element count";
    ASSERT_EQ(a({}), 0) << "failure in default ctor";
    ASSERT_EQ(b({2, 1, 1}), 0) << "failure in default value of ctor";
    ASSERT_EQ(c({2, 1, 1}), 42) << "failure in fill value of ctor";
    ASSERT_EQ(d.rank(), 10) << "failure in rank";
}

TEST(TensorTest, MoveSemantics){
    Tensor<int> a({2,2,2}, 2);
    auto b = a;
    ASSERT_EQ(a,b) << "failure in copy assignment";
    auto c = std::move(a);
    Tensor<int> d;
    ASSERT_EQ(a,d) << "failure in move assignment";
    ASSERT_EQ(c,b) << "failure in move constructor";

    a = b;
    Tensor<int> e(std::move(a));
    Tensor<int> f;
    ASSERT_EQ(a,f) << "incorrect satte after move assignment";
    ASSERT_EQ(e,b) << "failure in move assignment";
    
}

TEST(TensorTest, Access){
    Tensor<int> a({2,2,2}, 2);
    a({0,0,0}) = 1;
    a({0,0,1}) = 2;
    a({0,1,0}) = 3;
    a({0,1,1}) = 4;
    a({1,0,0}) = 5;
    a({1,0,1}) = 6;
    a({1,1,0}) = 7;
    a({1,1,1}) = 8;

    ASSERT_EQ(a({0,0,0}), 1) << "failure in access";
    ASSERT_EQ(a({0,0,1}), 2) << "failure in access";
    ASSERT_EQ(a({0,1,0}), 3) << "failure in access";
    ASSERT_EQ(a({0,1,1}), 4) << "failure in access";
    ASSERT_EQ(a({1,0,0}), 5) << "failure in access";
    ASSERT_EQ(a({1,0,1}), 6) << "failure in access";
    ASSERT_EQ(a({1,1,0}), 7) << "failure in access";
    ASSERT_EQ(a({1,1,1}), 8) << "failure in access";
}

TEST(TensorTest, fileio){
    auto a = readTensorFromFile< int >("data/tensor_01");
    ASSERT_EQ(a.rank(), 2) << "failure in rank";
    ASSERT_EQ(a({0, 0}), 1) << "failure in entry";
    ASSERT_EQ(a({0, 1}), 0) << "failure in entry";
    ASSERT_EQ(a({1, 0}), 0) << "failure in entry";
    ASSERT_EQ(a({1, 1}), -1) << "failure in entry";

    Tensor< int > b({2, 2, 2});
    b({0, 0, 0}) = 1;
    b({0, 0, 1}) = 2;
    b({0, 1, 0}) = 3;
    b({0, 1, 1}) = 4;
    b({1, 0, 0}) = 5;
    b({1, 0, 1}) = 6;
    b({1, 1, 0}) = 7;
    b({1, 1, 1}) = 8;
    
    writeTensorToFile< int >(b, "data/tensor_out");
    auto c = readTensorFromFile< int >("data/tensor_out");

    ASSERT_EQ(c({0, 0, 0}), 1) << "failure in write";
    ASSERT_EQ(c({0, 0, 1}), 2) << "failure in write";
    ASSERT_EQ(c({0, 1, 0}), 3) << "failure in write";
    ASSERT_EQ(c({0, 1, 1}), 4) << "failure in write";
    ASSERT_EQ(c({1, 0, 0}), 5) << "failure in write";
    ASSERT_EQ(c({1, 0, 1}), 6) << "failure in write";
    ASSERT_EQ(c({1, 1, 0}), 7) << "failure in write";
    ASSERT_EQ(c({1, 1, 1}), 8) << "failure in write";

    auto d = readTensorFromFile< int >("data/tensor_02");

    ASSERT_EQ(c,d) << "failure in read/write";
}