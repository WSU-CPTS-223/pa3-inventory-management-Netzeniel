#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <cstddef>

template <typename T>
class MyVector {
private:
    T* data;
    std::size_t _size;
    std::size_t _capacity;

    void reallocate(std::size_t newCap) {
        T* newData = new T[newCap];
        for (std::size_t i = 0; i < _size; ++i)
            newData[i] = data[i];
        delete[] data;
        data = newData;
        _capacity = newCap;
    }

public:
    MyVector()
        : data(nullptr), _size(0), _capacity(0) {}

    MyVector(const MyVector& other)
        : data(nullptr), _size(other._size), _capacity(other._capacity) {
        if (_capacity > 0) {
            data = new T[_capacity];
            for (std::size_t i = 0; i < _size; ++i)
                data[i] = other.data[i];
        }
    }

    MyVector& operator=(const MyVector& other) {
        if (this != &other) {
            delete[] data;
            _size = other._size;
            _capacity = other._capacity;

            if (_capacity > 0) {
                data = new T[_capacity];
                for (std::size_t i = 0; i < _size; ++i)
                    data[i] = other.data[i];
            } else {
                data = nullptr;
            }
        }
        return *this;
    }

    ~MyVector() {
        delete[] data;
    }

    void push_back(const T& value) {
        if (_size == _capacity) {
            std::size_t newCap = (_capacity == 0) ? 4 : _capacity * 2;
            reallocate(newCap);
        }
        data[_size++] = value;
    }

    T& operator[](std::size_t index) {
        return data[index];
    }

    const T& operator[](std::size_t index) const {
        return data[index];
    }

    std::size_t size() const {
        return _size;
    }

    bool empty() const {
        return _size == 0;
    }

    void clear() {
        _size = 0;
    }
};

#endif

