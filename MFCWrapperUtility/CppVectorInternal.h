#pragma once
#include <vector>
#include <cassert>
#include <iostream>
#include <random>
#include <string>
#include <future>
#include <thread>
#include <comutil.h>
template <typename T>
class CppVectorInternal
{
private:
    union VectorUnion
    {
        std::vector<T> vector;
        ~VectorUnion() {}
    };

private:
    VectorUnion* Vector;
    int (*Count)(VectorUnion*);
    void (*GetItemAt)(VectorUnion*, int, T*);
    void (*Dispose)(VectorUnion*);

public:
    CppVectorInternal(std::vector<T> vector) :
        Vector{ new VectorUnion { vector } },
        Count{ CountImpl },
        GetItemAt{ GetItemAtImpl },
        Dispose{ DisposeImpl }
    {
    }

private:
    static int CountImpl(VectorUnion* vector)
    {
        return static_cast<int>(vector->vector.size());
    }

    static void GetItemAtImpl(VectorUnion* vector, int idx, T* out)
    {
        assert(0 <= idx && idx < vector->vector.size());
        *out = vector->vector[idx];
    }

    static void DisposeImpl(VectorUnion* vector)
    {
        vector->vector.~vector();
        delete vector;
    }
};

struct NativeStruct
{
    int Value1;
    double Value2;
    float Value3;
};