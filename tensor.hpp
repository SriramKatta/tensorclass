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

// Pretty-prints the tensor to stdout.
// This is not necessary (and not covered by the tests) but nice to have, also for debugging (and for exercise of course...).
template <Arithmetic ComponentType>
std::ostream &
operator<<(std::ostream &out, const Tensor<ComponentType> &tensor)
{   
    return out;
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