#include "tensor.hpp"
#include <iostream>

int main()
{
    Tensor<int> mat2({2,2,2});
    mat2({0,0,0}) = 1;
    mat2({0,0,1}) = 2;
    mat2({0,1,0}) = 4;
    mat2({0,1,1}) = 4;
    mat2({1,0,0}) = 1+4;
    mat2({1,0,1}) = 2+4;
    mat2({1,1,0}) = 3+4;
    mat2({1,1,1}) = 4+4;

    std::cout << mat2 << std::endl;
    return 0;
}
