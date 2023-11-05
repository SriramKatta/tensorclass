
#pragma once

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
#include <numeric>

template <class T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <Arithmetic ComponentType>
class Tensor
{
public:
    // Constructs a tensor with rank = 0 and zero-initializes the element.
    Tensor() : mData(1), mRank(0){}

    // Constructs a tensor with arbitrary shape and zero-initializes all elements.
    Tensor(const std::vector<size_t> &shape):
    mData(std::accumulate(shape.begin(), shape.end(), 0, std::multiplies<int>())),
    mShape(shape){}

    // Constructs a tensor with arbitrary shape and fills it with the specified value.
    explicit Tensor(const std::vector<size_t> &shape, const ComponentType &fillValue):
    mData(std::accumulate(shape.begin(), shape.end(), 0, std::multiplies<int>()), fillValue),
    mShape(shape){}


    // Copy-constructor.
    Tensor(const Tensor<ComponentType> &other):
    mData(other.mData),
    mShape(other.mShape),
    mRank(other.mRank){}


    // Move-constructor.
    Tensor(Tensor<ComponentType> &&other) noexcept : 
    mData(std::move(other.mData)),
    mShape(std::move(other.mShape)),
    mRank(std::move(other.mRank)){}

    // Copy-assignment
    Tensor &
    operator=(const Tensor<ComponentType> &other){
        this->mData = other.mData;
        this->mShape = other.mShape;
        this->mRank = other.mRank;
        return *this;
    }

    // Move-assignment
    Tensor &
    operator=(Tensor<ComponentType> &&other) noexcept{
        this->mData = std::move(other.mData);
        this->mShape = std::move(other.mShape);
        this->mRank = std::move(other.mRank);
        return *this;
    }

    // Destructor
    ~Tensor() = default;

    // Returns the rank of the tensor.
    [[nodiscard]] size_t rank() const{return mRank;};

    // Returns the shape of the tensor.
    [[nodiscard]] std::vector<size_t> shape() const {return mShape;}

    // Returns the number of elements of this tensor.
    [[nodiscard]] size_t numElements() const {return mData.size();}

    // Element access function
    const ComponentType &
    operator()(const std::vector<size_t> &idx) const {        
        size_t loc{} ;
        for (size_t i = 0; i < idx.size(); i++)        {
            loc = idx[i] * mShape[i];
        }
        return mData[loc];
    }

    // Element mutation function
    ComponentType &
    operator()(const std::vector<size_t> &idx){
                size_t loc{} ;
        for (size_t i = 0; i < idx.size(); i++)        {
            loc = idx[i] * mShape[i];
        }
        return mData[loc];
    }

    auto& data()const{
        return mData;
    }
private:
    std::vector<ComponentType> mData;
    std::vector<size_t> mShape;
    size_t mRank;
};

// TODO: Implement all methods of the Tensor class template.

// Returns true if the shapes and all elements of both tensors are equal.
template <Arithmetic ComponentType>
bool operator==(const Tensor<ComponentType> &a, const Tensor<ComponentType> &b)
{
    if (a.rank() != b.rank())
    {
        return false;
    }
    
    return true;
}

// Pretty-prints the tensor to stdout.
// This is not necessary (and not covered by the tests) but nice to have, also for debugging (and for exercise of course...).
template <Arithmetic ComponentType>
std::ostream &
operator<<(std::ostream &out, const Tensor<ComponentType> &tensor)
{   
    auto& ldata = tensor.data();
    for(auto val : ldata){
        out << val << " ";
    }
    out << std::endl;
    return out;
}
/*
// Reads a tensor from file.
template <Arithmetic ComponentType>
Tensor<ComponentType> readTensorFromFile(const std::string &filename)
{
    // TODO: Implement this function to read in tensors from file.
    //       The format is defined in the instructions.
}

// Writes a tensor to file.
template <Arithmetic ComponentType>
void writeTensorToFile(const Tensor<ComponentType> &tensor, const std::string &filename)
{
    // TODO: Implement this function to write tensors to file.
    //       The format is defined in the instructions.
}
*/