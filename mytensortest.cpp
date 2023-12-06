#include "tensor.hpp"
#include <iostream>

int main()
{
    Tensor<int> mat({2,2});
    mat({0,0}) = 1;
    mat({0,1}) = 2;
    mat({1,0}) = 3;
    mat({1,1}) = 4;

    Tensor<int> mat3;

    Tensor<int> mat2({3,3},1);

    std::cout << (mat == mat2) << std::endl;
    return 0;
}
