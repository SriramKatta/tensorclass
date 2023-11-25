#include "tensor.hpp"
#include <iostream>

int main()
{
    Tensor<int> mat({2,2});
    std::cout << mat << std::endl;
    std::cout << "hello" << std::endl;
    return 0;
}
