/*--- 25/25 checks passed ---
************************************************************************************************
==2013== LEAK SUMMARY:
==2013==    definitely lost: 0 bytes in 0 blocks
==2013==    indirectly lost: 0 bytes in 0 blocks
==2013==      possibly lost: 0 bytes in 0 blocks
==2013==    still reachable: 188,344 bytes in 3 blocks
==2013==         suppressed: 0 bytes in 0 blocks
==2013==
==2013== For lists of detected and suppressed errors, rerun with: -s
==2013== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
Profiling timer expired
*/

#pragma once

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
#include <numeric>
#include <fstream>

template <class T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <Arithmetic ComponentType>
class Tensor
{
public:
    // Constructs a tensor with rank = 0 and zero-initializes the element.
    Tensor() : mData(1),mShape(){}

    // Constructs a tensor with arbitrary shape and zero-initializes all elements.
    Tensor(const std::vector<size_t> &shape):
    mData(std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<size_t>()),0),
    mShape(shape){}

    // Constructs a tensor with arbitrary shape and fills it with the specified value.
    explicit Tensor(const std::vector<size_t> &shape, const ComponentType &fillValue):
    mData(std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<size_t>()), fillValue),
    mShape(shape){}

    // Copy-constructor.
    Tensor(const Tensor<ComponentType> &other):
    mData(other.mData),
    mShape(other.mShape){}


    // Move-constructor.
    Tensor(Tensor<ComponentType> &&other) noexcept : 
    mData(std::move(other.mData)),
    mShape(std::move(other.mShape)){
        other = Tensor<ComponentType>();
    }

    // Copy-assignment
    Tensor &
    operator=(const Tensor<ComponentType> &other){
        this->mData = other.mData;
        this->mShape = other.mShape;
        return *this;
    }

    // Move-assignment
    Tensor &
    operator=(Tensor<ComponentType> &&other) noexcept{
        this->mData = std::move(other.mData);
        this->mShape = std::move(other.mShape);
        return *this;
    }

    // Destructor
    ~Tensor() = default;

    // Returns the rank of the tensor.
    [[nodiscard]] size_t rank() const{return mShape.size();};

    // Returns the shape of the tensor.
    [[nodiscard]] std::vector<size_t> shape() const {return mShape;}

    // Returns the number of elements of this tensor.
    [[nodiscard]] size_t numElements() const {return mData.size();}

    // Element access function
    const ComponentType &
    operator()(const std::vector<size_t> &idx) const {        
        return mData[indextoloc(idx)];
    }

    // Element mutation function
    ComponentType &
    operator()(const std::vector<size_t> &idx){
        return mData[indextoloc(idx)];
    }
    bool operator==(const Tensor<ComponentType> &b) const {
        auto& a = *this; 
        return a.mShape == b.mShape && a.mData == b.mData;
    }
private:
    size_t indextoloc(const std::vector<size_t>& idx) const {
        size_t mul{1}, loc{0};
        for (int i = mShape.size() - 1; i >= 0; --i)
        {
            loc += idx[i] * mul;
            mul *= mShape[i];
        }
        return loc;
    }
    std::vector<ComponentType> mData;
    std::vector<size_t> mShape;
};

// TODO: Implement all methods of the Tensor class template.

// Returns true if the shapes and all elements of both tensors are equal.
template <Arithmetic ComponentType>
bool operator==(const Tensor<ComponentType> &a, const Tensor<ComponentType> &b){
    return a == b;
}

// Reads a tensor from file.
template <Arithmetic ComponentType>
Tensor<ComponentType> readTensorFromFile(const std::string &filename)
{
    std::ifstream tendata(filename);
    size_t rank{0},val{0};
    tendata >> rank;
    std::vector<size_t> shape;
    for (size_t i = 0; i < rank; i++){
        tendata >> val;
        shape.push_back(val);
    }
    Tensor<ComponentType> res(shape);
    ComponentType dataval;
    auto loctoindex = [&shape](size_t loc)
    {
        std::vector<size_t> idx(shape.size());
        size_t mul{1};
        for (int i = shape.size() - 1; i >= 0; --i)
        {
            idx[i] = (loc / mul) % shape[i];
            mul *= shape[i];
        }
        return idx;
    };
    for (size_t i = 0; i < res.numElements(); ++i)
    {
        tendata >> dataval;
        const auto idx = loctoindex(i);
        res(idx) = dataval;   
    }
    return res;
}

// Writes a tensor to file.
template <Arithmetic ComponentType>
void writeTensorToFile(const Tensor<ComponentType> &tensor, const std::string &filename)
{
    std::ofstream tenfile(filename);
    tenfile << tensor.rank() << std::endl;
    for(auto val : tensor.shape()){
        tenfile << val << std::endl;
    }

    auto shape = tensor.shape();
    auto loctoindex = [&shape](size_t loc)
    {
        std::vector<size_t> idx(shape.size());
        size_t mul{1};
        for (int i = shape.size() - 1; i >= 0; --i)
        {
            idx[i] = (loc / mul) % shape[i];
            mul *= shape[i];
        }
        return idx;
    };
    for (size_t i = 0; i < tensor.numElements(); ++i)
    {
        const auto idx = loctoindex(i);
        tenfile << tensor(idx) << std::endl;  
    }
    
}


// Pretty-prints the tensor to stdout.
// This is not necessary (and not covered by the tests) but nice to have, also for debugging (and for exercise of course...).
template <Arithmetic ComponentType>
std::ostream &
operator<<(std::ostream &out, const Tensor<ComponentType> &tensor)
{   
    printtensor(out, tensor, tensor.shape());
    return out;
}
template <Arithmetic ComponentType>
void printtensor(std::ostream &out, const Tensor<ComponentType> &tensor,
                            const std::vector<size_t> &shape, std::vector<size_t> currentIndices = {}, size_t dimension = 0)
{
    const size_t numElements = shape[dimension];

    out << "Dimension " << dimension << " (" << numElements << " elements):" << std::endl;

    // Indentation for nested dimensions.
    std::string indentation(dimension * 2, ' ');

    for (size_t i = 0; i < numElements; ++i)
    {
        currentIndices.push_back(i);
        if (dimension < shape.size() - 1)
        {
            // If not the last dimension, recursively print the inner dimension.
            out << indentation << "Element " << i << ":" << std::endl;
            printtensor(out, tensor, shape, currentIndices, dimension + 1);
        }
        else
        {
            // If the last dimension, print the value.
            out << indentation << "Element " << i << ": " << tensor(currentIndices) << std::endl;
        }
        currentIndices.pop_back();
    }
}