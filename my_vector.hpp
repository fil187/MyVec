#include <cstddef>
#include <stdexcept>
    
template <typename T>
class MyVector {

private:

    T* data;
    size_t capacity;
    size_t size;
    static constexpr size_t DEFAULT_CAPACITY = 100;

public:

    MyVector() : MyVector(DEFAULT_CAPACITY) {
        // constructor with default capacity (100 elements)
    }
    
    MyVector(size_t capacity) : capacity(capacity), size(0) {
        // constructor with defined capacity
        // throws illegal argument exception if capacity is null
        // @param capacity -> capcity for the vector
        if (capacity == 0)
            throw std::invalid_argument("capacity must be greater than 0");

        data = new T[capacity];
    }

    MyVector(const T* source, size_t size) : capacity(max(size << 1, DEFAULT_CAPACITY)), size(size) {
        // constructor from array
        // @param source -> the array from wich to build the vector from
        // @param size -> the size of the array
        data = new T[max(size << 1, DEFAULT_CAPACITY)];
        for (size_t i = 0; i < size; i++)
            data[i] = source[i];
    }

    MyVector(const MyVector& source) : capacity(source.capacity), size(source.size) {
        // construct from other MyVector Object
        // @param source -> the vector to build this vector from
        data = new T[source.capacity];
        for (size_t i = 0; i < source.size; i++)
            data[i] = source.data[i];
    }

    T& operator[](size_t i) {
        if (i >= size)
            throw std::out_of_range("index out of range");

        return data[i];
    }

    MyVector& operator=(const MyVector& source) {
        if (this == &source)
            return *this;
        
        delete[] data;
        data = new T[source.capacity];
        capacity = source.capacity;
        size = source.size;

        for (size_t i = 0; i < source.size; i++)
            data[i] = source.data[i];
        
        return *this;
    }

    size_t length() const {
        return size;
    }

    void push(const T& element) {
        if (size == capacity)
            resize(max(1, 2 * capacity));
        
        data[size++] = element;
    }

    T pop() {
        if (size == 0)
            throw std::out_of_range("can not pop when vector is empty");
        if (size == capacity >> 2) // >> 2 -> 1/4
            resize(capacity >> 1); // >> 1 -> 1/2
        
        return data[--size];
    }

    void clear() {
        // sets the size of the array to 0
        size = 0;
    }

    void sort() requires std::totally_ordered<T> {
        // calls private sort method for the whole array
        sort(0, size - 1);
    }

    ~MyVector() {
        delete[] data;
    }
private:


    static inline size_t max(size_t a, size_t b) {
        return (a >= b) ? a : b;
    }

    void resize(size_t new_capacity) {
        T* destination = new T[new_capacity];
        for (size_t i = 0; i < size; i++)
            destination[i] = data[i];
        
        delete[] data;
        data = destination;
        capacity = new_capacity;
    }

    void sort(size_t left, size_t right) {
        if (left >= right)
            return;

        size_t pivot_idx = left + ((right - left) >> 1);
        T pivot = data[pivot_idx];
        std::swap(data[pivot_idx], data[right]);
        
        size_t lower_bound = left;

        for (size_t i = left; i < right; i++)
            if (data[i] <= pivot)
                std::swap(data[i], data[lower_bound++]);
        
        std::swap(data[lower_bound], data[right]);
        sort(left, (lower_bound > 0) ? lower_bound - 1 : 0);
        sort(lower_bound + 1, right);
    }
};