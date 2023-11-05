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

#pragma once

#include "tensor.hpp"

template< typename ComponentType >
class Vector
{
public:
    // Default-constructor.
    Vector() = default;

    // Constructor for vector of certain size.
    explicit Vector(size_t size):tensor_({size}){}

    // Constructor for vector of certain size with constant fill-value.
    Vector(size_t size, const ComponentType& fillValue):tensor_({size}, fillValue){}

    // Constructing vector from file.
    Vector(const std::string& filename):tensor_(readTensorFromFile<ComponentType>(filename)){}

    // Number of elements in this vector.
    [[nodiscard]] size_t size() const{
        return tensor_.numElements();
    }

    // Element access function
    const ComponentType&
    operator()(size_t idx) const{
        return tensor_({idx,1});
    }

    // Element mutation function
    ComponentType&
    operator()(size_t idx){
        return tensor_({idx,0});
    }

    // Reference to internal tensor.
    Tensor< ComponentType >& tensor(){
        return tensor_;
    }

private:
    Tensor< ComponentType > tensor_;
};

template< typename ComponentType >
class Matrix
{
public:
    // Default-constructor.
    Matrix() = default;

    // Constructor for matrix of certain size.
    explicit Matrix(size_t rows, size_t cols):tensor_({rows, cols}){}

    // Constructor for matrix of certain size with constant fill-value.
    Matrix(size_t rows, size_t cols, const ComponentType& fillValue):tensor_({rows, cols}, fillValue){}

    // Constructing matrix from file.
    Matrix(const std::string& filename):tensor_(readTensorFromFile<ComponentType>(filename)){}

    // Number of rows.
    [[nodiscard]] size_t rows() const{
        return tensor_.shape()[0];
    }

    // Number of columns.
    [[nodiscard]] size_t cols() const{
        return tensor_.shape()[1];
    }

    // Element access function
    const ComponentType&
    operator()(size_t row, size_t col) const{
        return tensor_({row,col});
    }
    // Element mutation function
    ComponentType&
    operator()(size_t row, size_t col){
        return tensor_({row,col});
    }

    // Reference to internal tensor.
    Tensor< ComponentType >& tensor(){
        return tensor_;
    }

private:
    Tensor< ComponentType > tensor_;
};

// TODO: Implement all methods.


// Performs a matrix-vector multiplication.
template< typename ComponentType >
Vector< ComponentType > matvec(const Matrix< ComponentType >& mat, const Vector< ComponentType >& vec)
{
    if(mat.cols() != vec.size()){
        std::cout << "size mismatch" << std::endl;
        return {};
    }
    auto ressize = vec.size();
    Vector<ComponentType> res(ressize);

    for (size_t i = 0; i < ressize; ++i){
        for (size_t idx = 0; idx < ressize; ++idx)
        {
            //std::cout << "res = " << res(i) << " mat(i,idx) = " << mat(i,idx) << " vec(idx) =  " << vec(idx) <<std::endl;
            res(i) = res(i) + mat(i,idx)*vec(idx);
        }
    }
    return res;
}

