#include <cstddef>
#include <stdexcept>

/**
 * @brief A dynamically allocated mutable array.
 * 
 * @tparam T must be copyable.
 * 
 * @invariant `capacity > 0`
 * @invariant `size <= capacity`
 * @invariant `data != nullptr`
 * @invariant `size` is always greater then 25% of capacity iff `capacity <= DEFAULT_CAPACITY`
 * @invariant data points to an allocated array of exactly `capacity` elements `T`.
 */
template <std::copyable T>
class MyVector {

private:

    T* data;
    size_t capacity;
    size_t size;
    static constexpr size_t DEFAULT_CAPACITY = 100;

public:

    /**
     * @brief Construct an empty vector with the default capacity.
     * 
     * @throw std::bad_alloc if the allocation fails
     */
    MyVector() : MyVector(DEFAULT_CAPACITY) {}
    
    /**
     * @brief Construct an empty vector with the specified capacity.
     * 
     * @param capacity The initial number of elements that can be stored without reallocation.
     * @throw std::invalid_argument if `capacity == 0`
     * @throw std::bad_alloc if the allocation fails
     */
    MyVector(size_t capacity) : capacity(capacity), size(0) {
        if (capacity == 0)
            throw std::invalid_argument("capacity must be greater than 0");

        data = new T[capacity];
    }

    /**
     * @brief Constructs a vector containing copies of the elements in `source`.
     * 
     * @par Complexity
     *      O(m)
     * 
     * @throw std::invalid_argument if `source == nullptr`
     * @throw std::bad_alloc if the allocation fails
     * @param source The array whose contents are copied into this vector.
     * @param size The number of elements in `source`.
     */
    MyVector(const T* source, size_t size) : capacity(std::max(size << 1, DEFAULT_CAPACITY)), size(size) {
        if (source == nullptr)
            throw std::invalid_argument("source can not be NULL");
        
        data = new T[std::max(size << 1, DEFAULT_CAPACITY)];
        std::copy(source, source + size, data);
    }

    /**
     * Constructs a vector containing copies of the elements in `source`.
     * @brief Copy Constructor.
     * 
     * @par Complexity
     *      O(m)
     * 
     * @throw std::bad_alloc if the allocation fails
     * @param source The vector whose contents are copied into this vector.
     */
    MyVector(const MyVector& source) : capacity(source.capacity), size(source.size) {
        data = new T[source.capacity];
        std::copy(source.data, source.data + source.size, data);
    }

    /**
     * @note Move operations are intentionally disabled because 
     *       this container's invariants do not permit a moved-from state.
     */
    MyVector(MyVector&&) = delete;

    /**
     * @note Move operations are intentionally disabled because 
     *       this container's invariants do not permit a moved-from state.
     */
    MyVector& operator=(MyVector&&) = delete;

    /**
     * @throw std::out_of_range if i greater than or equal to the length of this vector.
     * @return the element at the specified index.
     */
    T& operator[](size_t i) {
        if (i >= size)
            throw std::out_of_range("index out of range");

        return data[i];
    }

    /**
     * Replaces the contents of this vector with those of `source`.
     * @brief Copy Assignment Operator.
     * 
     * @par Complexity
     *      Worst case O(n)
     * 
     * @throw std::bad_alloc if the allocation fails
     * 
     * @post The contents of this vector are equal to `source`.
     * @post The length of this vector equals the length of `source`.
     */
    MyVector& operator=(const MyVector& source) {
        if (this == &source)
            return *this;
        
        delete[] data;
        data = new T[source.capacity];
        capacity = source.capacity;
        size = source.size;

        std::copy(source.data, source.data + source.size, data);
        
        return *this;
    }

    /**
     * @return The number of stored elements.
     */
    size_t length() const {
        return size;
    }

    /**
     * @brief Adds the element at the end of the vector.
     * 
     * @par Complexity
     *      Worst case O(n)
     *      Average case O(1)
     * 
     * @post The length of this vector is increased by 1.
     * @post `element` is the last element of the vector.
     * @post The rest of the vector remains unchanged.
     */
    void push(const T& element) {
        if (size == capacity)
            resize(std::max((size_t)1, 2 * capacity));
        
        data[size++] = element;
    }

    /**
     * @brief Removes the last element of the vector.
     * 
     * @par Complexity
     *      Worst case O(n)
     *      Average case O(1)
     * 
     * @throw std::out_of_range if the vector is empty
     * @return The removed element.
     * 
     * @post The rest of the vector remains unchanged.
     * @post The length of this vector is decreased by 1.
     */
    T pop() {
        if (size == 0)
            throw std::out_of_range("can not pop when vector is empty");
        if (capacity > DEFAULT_CAPACITY && size == capacity >> 2) // >> 2 -> 1/4
            resize(capacity >> 1); // >> 1 -> 1/2
        
        return data[--size];
    }

    /**
     * @brief Sorts the vector in increasing order
     * @note This function is only available when `T` models `std::totally_ordered`.
     * 
     * @par Complexity
     *      Average case O(n * log n)
     * 
     * @post The vector is sorted in increasing order.
     * @post All elements originally in data are present in the same number.
     */
    void sort() requires std::totally_ordered<T> {
        sort(0, size - 1);
    }

    ~MyVector() {
        delete[] data;
    }
    
private:

    /**
     * @par Complexity
     *      Worst case O(n)
     * 
     * @throw std::bad_alloc if the allocation fails
     * 
     * @pre `new_capacity > 0`
     * @post The contents of this vector are unchanged
     * @post The capacity of this vector is equal to new_capacity
     */
    void resize(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        std::copy(data, data + size, new_data);
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }

    /**
     * @par Complexity
     *      Average case O(n * log n)
     * 
     * @post `data[left..right]` is sorted.
     * @post All elements originally in data are present in the same number.
     */
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