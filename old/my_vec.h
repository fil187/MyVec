#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#ifndef MY_VEC_H
#define MY_VEC_H

struct MyVec {
    size_t capacity;            // in bytes
    size_t size;                // in bytes
    size_t element_size;        // size of the elemnts stored, needed to retrive the data and store it

    uint8_t* data;              // data stored as bytes
};

typedef struct MyVec* MyVec;

#define DEFAULT_CAPACITY 100
#define LARGE_CHUNK_SIZE 8

#define SIZE_8 1
#define SIZE_16 2
#define SIZE_32 4
#define SIZE_64 8

/*##########################################################
#                         functions                        #
##########################################################*/
MyVec vec_new_with_capacity(size_t element_size, size_t capacity); // constructor with specified capacity (capacity * element_size)
MyVec vec_new(size_t element_size);                                // constructor with default capacity (100 * element_size)
MyVec vec_from(void* data, size_t element_size, size_t data_size); // constructor from existing array
size_t len(MyVec self);                                            // returns the number of items in the vec
void push(MyVec self, void* element);                              // add element to the end of the vector
uint8_t* get(MyVec self, size_t idx);                              // get elemnt at index (byte pointer | needs casting)
uint8_t* pop(MyVec self);                                          // pops the last element of the vector, returns the element that was removed (byte pointer | needs casting)
int clear(MyVec _self);                                            // *empty the vec (size is set back to 0) data is still present in memory returns 1 on success
int append(MyVec self, MyVec source);                              // append another my_vec to the first argument: my_vec
void destroy(MyVec self);                                          // destroyer
void sort(MyVec self, int is_signed);                              // sort function (only for vector of rvalue (u8 / char, u16, u32, u64, i8, i16, i32, i64))

/*##########################################################
#               basic casting and referencing              #
##########################################################*/ 
// casting for get (cleaner code)
#define GET_INT(SELF, IDX) *(int*)get(SELF, IDX)
#define GET_FLOAT(SELF, IDX) *(float*)get(SELF, IDX)
#define GET_CHAR(SELF, IDX) *(char*)get(SELF, IDX)
#define GET_U8(SELF, IDX) *(uint8_t*)get(SELF, IDX)
#define GET_U16(SELF, IDX) *(uint16_t*)get(SELF, IDX)
#define GET_U32(SELF, IDX) *(uint32_t*)get(SELF, IDX)
#define GET_U64(SELF, IDX) *(uint64_t*)get(SELF, IDX)
#define GET_I8(SELF, IDX) *(int8_t*)get(SELF, IDX)
#define GET_I16(SELF, IDX) *(int16_t*)get(SELF, IDX)
#define GET_I32(SELF, IDX) *(int32_t*)get(SELF, IDX)
#define GET_I64(SELF, IDX) *(int64_t*)get(SELF, IDX)
#define GET_SIZE(SELF, IDX) *(size_t*)get(SELF, IDX)
// casting for pop (cleaner code)
#define POP_INT(SELF) *(int*)pop(SELF)
#define POP_FLOAT(SELF) *(float*)pop(SELF)
#define POP_CHAR(SELF) *(char*)pop(SELF)
#define POP_U8(SELF) *(uint8_t*)pop(SELF)
#define POP_U16(SELF) *(uint16_t*)pop(SELF)
#define POP_U32(SELF) *(uint32_t*)pop(SELF, IDX)
#define POP_U64(SELF) *(uint64_t*)pop(SELF, IDX)
#define POP_I8(SELF) *(int8_t*)pop(SELF, IDX)
#define POP_I16(SELF) *(int16_t*)pop(SELF, IDX)
#define POP_I32(SELF) *(int32_t*)pop(SELF, IDX)
#define POP_I64(SELF) *(int64_t*)pop(SELF, IDX)
#define POP_SIZE(SELF) *(size_t*)pop(SELF, IDX)

#endif