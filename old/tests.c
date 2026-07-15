#include <stdio.h>
#include <assert.h>
#include "my_vec.h"

int main() {
    
    /*##########################################################
    #                   tests for MyVec<int>                   #
    ##########################################################*/
    
    printf("\033[34m");

    //constructor tests:
    MyVec v1 = vec_new(sizeof(int));
    assert(v1 != NULL);
    assert(v1->capacity == DEFAULT_CAPACITY * sizeof(int));
    assert(v1->size == 0);
    assert(v1->element_size == sizeof(int));
    assert(v1->data != NULL);
    printf("default constructor test past\n");
    MyVec v2 = vec_new_with_capacity(sizeof(int), 150);
    assert(v2 != NULL);
    assert(v2->capacity == 150 * sizeof(int));
    assert(v2->size == 0);
    assert(v2->element_size == sizeof(int));
    assert(v2->data != NULL);
    printf("constructor with capacity test past\n");
    int data_3[4] = {1, 2, 3, 4};
    MyVec v3 = vec_from(data_3, sizeof(int), sizeof(data_3));
    assert(v3->capacity == DEFAULT_CAPACITY * sizeof(int));   // since 4 < DEFAULT_CAPACITY
    assert(len(v3) == 4);
    assert(v3->element_size == sizeof(int));
    assert(v3->data != NULL);
    printf("constructor from array test past\n");
    printf("\033[36mconstructor tests past\033[34m\n\n");

    // push test:
    int val1 = 15;
    push(v2, &val1);
    assert(len(v2) == 1);
    printf("single value push test past\n");
    for (int i = 0; i < 201; i++) {
        //force resize
        push(v1, &i);
    }
    printf("multiple value push test past\n");
    printf("resize test past\n");
    assert(v1->capacity > DEFAULT_CAPACITY);
    assert(len(v1) == 201);
    printf("\033[36mpush tests past\033[34m\n\n");
    
    // get tests:
    assert(GET_INT(v2, 0) == 15);
    for (int i = 0; i < 201; i++) {
        assert(GET_INT(v1, i) == i);
    }
    printf("\033[36mget tests past\033[34m\n\n");

    // pop tests:
    assert(POP_INT(v2) == 15);
    assert(len(v2) == 0);
    assert(POP_INT(v1) == 200);
    assert(len(v1) == 200);
    printf("\033[36mpop tests past\033[34m\n\n");

    // clear tests:
    clear(v3);
    assert(len(v2) == 0);
    assert(v3->capacity >= DEFAULT_CAPACITY * sizeof(int));
    printf("\033[36mclear test past\033[34m\n\n");

    //append tests:
    int val2 = -1;
    push(v2, &val2);
    append(v2, v1);     //should force resize of v2
    for (int i = 0; i < 201; i++) {
        assert(GET_INT(v2, i) == i - 1);
    }
    printf("resize for append test past\n");
    printf("\033[36mappend test past\033[34m\n\n");
    
    // destructor tests:
    destroy(v1);
    destroy(v2);
    destroy(v3);
    printf("\033[36mdestructor tests past\033[34m\n\n");

    /*##########################################################
    #            tests for sorting implementations             #
    ##########################################################*/
    // arrays of all number in the range(0, 49) inclusief
    uint8_t u8_array[50] = {23, 1, 47, 12, 35, 0, 19, 44, 8, 31, 4, 26, 14, 39, 6, 28, 49, 17, 2, 41, 10, 33, 21, 45, 7, 29, 16, 38, 24, 5, 42, 11, 34, 27, 9, 40, 18, 46, 3, 32, 13, 20, 22, 25, 30, 36, 37, 43, 48, 15};
    uint16_t u16_array[50] = {23, 1, 47, 12, 35, 0, 19, 44, 8, 31, 4, 26, 14, 39, 6, 28, 49, 17, 2, 41, 10, 33, 21, 45, 7, 29, 16, 38, 24, 5, 42, 11, 34, 27, 9, 40, 18, 46, 3, 32, 13, 20, 22, 25, 30, 36, 37, 43, 48, 15};
    uint32_t u32_array[50] = {23, 1, 47, 12, 35, 0, 19, 44, 8, 31, 4, 26, 14, 39, 6, 28, 49, 17, 2, 41, 10, 33, 21, 45, 7, 29, 16, 38, 24, 5, 42, 11, 34, 27, 9, 40, 18, 46, 3, 32, 13, 20, 22, 25, 30, 36, 37, 43, 48, 15};
    uint64_t u64_array[50] = {23, 1, 47, 12, 35, 0, 19, 44, 8, 31, 4, 26, 14, 39, 6, 28, 49, 17, 2, 41, 10, 33, 21, 45, 7, 29, 16, 38, 24, 5, 42, 11, 34, 27, 9, 40, 18, 46, 3, 32, 13, 20, 22, 25, 30, 36, 37, 43, 48, 15};
    // arrays of all number in the range(-25, 24) inclusief
    int8_t i8_array[50] = {7, -14, 23, -1, -25, 12, -9, 0, 19, -6, 4, -18, 16, -3, 21, -11, 9, -22, 2, 14, -7, 24, -15, 6, -20, 11, -4, 18, -10, 1, 22, -17, 5, -2, 15, -8, 20, -24, 8, -13, 3, 10, -21, 17, -5, 13, -19,  -12, -23, -16};
    int16_t i16_array[50] = {7, -14, 23, -1, -25, 12, -9, 0, 19, -6, 4, -18, 16, -3, 21, -11, 9, -22, 2, 14, -7, 24, -15, 6, -20, 11, -4, 18, -10, 1, 22, -17, 5, -2, 15, -8, 20, -24, 8, -13, 3, 10, -21, 17, -5, 13, -19,  -12, -23, -16};
    int32_t i32_array[50] = {7, -14, 23, -1, -25, 12, -9, 0, 19, -6, 4, -18, 16, -3, 21, -11, 9, -22, 2, 14, -7, 24, -15, 6, -20, 11, -4, 18, -10, 1, 22, -17, 5, -2, 15, -8, 20, -24, 8, -13, 3, 10, -21, 17, -5, 13, -19,  -12, -23, -16};
    int64_t i64_array[50] = {7, -14, 23, -1, -25, 12, -9, 0, 19, -6, 4, -18, 16, -3, 21, -11, 9, -22, 2, 14, -7, 24, -15, 6, -20, 11, -4, 18, -10, 1, 22, -17, 5, -2, 15, -8, 20, -24, 8, -13, 3, 10, -21, 17, -5, 13, -19,  -12, -23, -16};
    // making the vectors:
    MyVec u8_vec = vec_from(u8_array, sizeof(uint8_t), sizeof(u8_array));
    MyVec u16_vec = vec_from(u16_array, sizeof(uint16_t), sizeof(u16_array));
    MyVec u32_vec = vec_from(u32_array, sizeof(uint32_t), sizeof(u32_array));
    MyVec u64_vec = vec_from(u64_array, sizeof(uint64_t), sizeof(u64_array));
    MyVec i8_vec = vec_from(i8_array, sizeof(int8_t), sizeof(i8_array));
    MyVec i16_vec = vec_from(i16_array, sizeof(int16_t), sizeof(i16_array));
    MyVec i32_vec = vec_from(i32_array, sizeof(int32_t), sizeof(i32_array));
    MyVec i64_vec = vec_from(i64_array, sizeof(int64_t), sizeof(i64_array));
    
    // sorting the vectors:
    sort(u8_vec, 0);
    printf("sort for u8\n");
    sort(u16_vec, 0);
    printf("sort for u16\n");
    sort(u32_vec, 0);
    printf("sort for u32\n");
    sort(u64_vec, 0);
    printf("sort for u64\n");
    sort(i8_vec, 1);
    printf("sort for i8\n");
    sort(i16_vec, 1);
    printf("sort for i16\n");
    sort(i32_vec, 1);
    printf("sort for i32\n");
    sort(i64_vec, 1);
    printf("sort for i64\n");

    // testing the sort impl:
    // unsigned:
    for (int i = 0; i < 50; i++) {
        assert(GET_U8(u8_vec, i) == i);
    }
    for (int i = 0; i < 50; i++) {
        assert(GET_U16(u16_vec, i) == i);
    }
    for (int i = 0; i < 50; i++) {
        assert(GET_U32(u32_vec, i) == i);
    }
    for (int i = 0; i < 50; i++) {
        assert(GET_U64(u64_vec, i) == i);
    }
    // signed:
    for (int i = 0; i < 50; i++) {
        assert(GET_I8(i8_vec, i) == i - 25);
    }
    for (int i = 0; i < 50; i++) {
        assert(GET_I16(i16_vec, i) == i - 25);
    }
    for (int i = 0; i < 50; i++) {
        assert(GET_I32(i32_vec, i) == i - 25);
    }
    for (int i = 0; i < 50; i++) {
        assert(GET_I64(i64_vec, i) == i - 25);
    }
    printf("\033[36msorting implementation tests past\033[0m\n\n");

    destroy(u8_vec);
    destroy(u16_vec);
    destroy(u32_vec);
    destroy(u64_vec);
    destroy(i8_vec);
    destroy(i16_vec);
    destroy(i32_vec);
    destroy(i64_vec);

    return 0;
}