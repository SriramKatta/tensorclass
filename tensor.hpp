/*
 * tensor.hpp
 *
 *  Created on: 10.11.2023
 *      modified : 10.12.2023
 *      Author: Sriram Katta
 *      c++std  : C++20
 */
#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <fstream>
#include <numeric>


template< class T >
concept Arithmetic = std::is_arithmetic_v< T >;

template< Arithmetic ComponentType >
class Tensor
{
public:
    // Constructs a tensor with rank = 0 and zero-initializes the element.
    Tensor():
    mShape(),
    mData(1,0),
    mStride(){}

    // Constructs a tensor with arbitrary shape and zero-initializes all elements.
    Tensor(const std::vector< size_t >& shape):
    Tensor(shape, 0){}

    // Constructs a tensor with arbitrary shape and fills it with the specified value.
    explicit Tensor(const std::vector< size_t >& shape, const ComponentType& fillValue):
    mShape(shape),
    mData(std::accumulate(mShape.begin(),mShape.end(),1,std::multiplies<size_t>()), fillValue),
    mStride(stridecalc()){}

    // Copy-constructor.
    Tensor(const Tensor< ComponentType >& other):
    mShape(other.mShape),
    mData(other.mData),
    mStride(other.mStride){}

    // Move-constructor.
    Tensor(Tensor< ComponentType >&& other) noexcept:
    mShape(std::move(other.mShape)),
    mData(std::move(other.mData)),
    mStride(std::move(other.mStride)){
        other = Tensor();
    }

    // Copy-assignment
    Tensor&
    operator=(const Tensor< ComponentType >& other){
        this->mData = other.mData;
        this->mShape = other.mShape;
        this->mStride = other.mStride;
        return *this;
    }

    // Move-assignment
    Tensor&
    operator=(Tensor< ComponentType >&& other) noexcept{
        this->mData = std::move(other.mData);
        this->mShape = std::move(other.mShape);
        this->mStride = std::move(other.mStride);
        return *this;
    }

    // Destructor
    ~Tensor() = default;

    // Returns the rank of the tensor.
    [[nodiscard]] size_t rank() const { return mShape.size(); }

    // Returns the shape of the tensor.
    [[nodiscard]] std::vector< size_t > shape() const { return mShape; }

    // Returns the number of elements of this tensor.
    [[nodiscard]] size_t numElements() const { return mData.size(); }

    // Element access function
    const ComponentType&
    operator()(const std::vector< size_t >& idx) const{
        auto loc = idxtoindex(idx);
        return mData[loc];
    }

    // Element mutation function
    ComponentType&
    operator()(const std::vector< size_t >& idx){
        auto loc = idxtoindex(idx);
        return mData[loc];
    }

private:
    std::vector<size_t> mShape;
    std::vector<ComponentType> mData;
    std::vector<size_t> mStride; 

    std::vector<size_t> stridecalc() const {
        std::vector<size_t> res;
        res.reserve(mShape.size());
        res.push_back(1);
        for (size_t i = 1; i < mShape.size() ;++i)
        {
            res.push_back(res.back() * mShape[i]);
        }
        std::reverse(res.begin(),res.end());
        return res;
    }

    size_t idxtoindex(const std::vector<size_t>& idx) const {
        return std::inner_product(mStride.begin(),mStride.end(),idx.begin(),0);
    }

    template< Arithmetic DataType >
    friend Tensor< DataType > readTensorFromFile(const std::string&);
    template< Arithmetic DataType >
    friend void writeTensorToFile(const Tensor< DataType >&, const std::string&);
    template< Arithmetic DataType >
    friend bool operator==(const Tensor< DataType >& a, const Tensor< DataType >& b);

};

// TODO: Implement all methods of the Tensor class template.


// Returns true if the shapes and all elements of both tensors are equal.
template< Arithmetic DataType >
bool operator==(const Tensor< DataType >& a, const Tensor< DataType >& b)
{
    return a.mShape == b.mShape && a.mData == b.mData;
}

// Reads a tensor from file.
template< Arithmetic DataType >
Tensor< DataType > readTensorFromFile(const std::string& filename)
{
    std::ifstream tensorfile(filename);
    size_t rank{};
    tensorfile >> rank;
    std::vector<size_t> shape;
    shape.reserve(rank);
    size_t fileval{};
    for (size_t i = 0; i < rank; i++){
        tensorfile >> fileval;
        shape.push_back(fileval);
    }
    Tensor<DataType> res(shape);
    for (auto &loc : res.mData)
    {
        tensorfile >> fileval;
        loc = std::move(fileval);
    }
    return res;
}

// Writes a tensor to file.
template< Arithmetic DataType >
void writeTensorToFile(const Tensor< DataType >& tensor, const std::string& filename)
{
    std::ofstream tensorfile(filename);
    tensorfile << tensor.rank() << std::endl;
    for (auto& shapeval : tensor.mShape){
        tensorfile << shapeval <<std::endl;
    }
    for (auto& shapeval : tensor.mData){
        tensorfile << shapeval <<std::endl;
    }    
}


// Pretty-prints the tensor to stdout.
// This is not necessary (and not covered by the tests) but nice to have, also for debugging (and for exercise of course...).

template <Arithmetic DataType>
std::ostream &operator<<(std::ostream &out,
                         const Tensor<DataType> &tensor) {
  printtensor(out, tensor, tensor.shape());
  return out;
}

template <Arithmetic DataType>
void printtensor(std::ostream &out, const Tensor<DataType> &tensor,
                 const std::vector<size_t> &shape,
                 std::vector<size_t> currentIndices = {},
                 size_t dimension = 0) {

    if (shape.size() == 1)
    {
        for (size_t i = 0; i < shape[0]; i++)
        {
            out << tensor({i}) << " ";
        }
        out << std::endl;
        return;
    }

    if (shape.size() == 2)
    {
        for (size_t row = 0; row < shape[0]; row++)
        {
            for (size_t col = 0; col < shape[1]; col++)
            {
                out << tensor({row, col}) << " ";
            }
            out << std::endl;
        }
        return;
    }

  const size_t numElements = shape[dimension];

  out << "Dimension " << dimension << " (" << numElements
      << " elements):" << std::endl;

  // Indentation for nested dimensions.
  std::string indentation(dimension * 2, ' ');

  for (size_t i = 0; i < numElements; ++i) {
    currentIndices.push_back(i);
    if (dimension < shape.size() - 1) {
      // If not the last dimension, recursively print the inner dimension.
      out << indentation << "Element " << i << ":" << std::endl;
      printtensor(out, tensor, shape, currentIndices, dimension + 1);
    } else {
      // If the last dimension, print the value.
      out << indentation << "Element " << i << ": " << tensor(currentIndices)
          << std::endl;
    }
    currentIndices.pop_back();
  }
}