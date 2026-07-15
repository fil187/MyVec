#include "my_vec.h"

/*##########################################################
#                         functions                        #
##########################################################*/

MyVec vec_new_with_capacity(size_t element_size, size_t capacity) {
    MyVec self = malloc(sizeof(struct MyVec));
    if (self == NULL) {
        return NULL;
    }

    self->element_size = element_size;
    self->capacity = capacity * element_size;
    self->size = 0;

    self->data = malloc(self->capacity * element_size);
    if (self->data == NULL) {
        free(self);
        return NULL;
    }

    return self;
}

MyVec vec_new(size_t element_size) {
    return vec_new_with_capacity(element_size, DEFAULT_CAPACITY);
}

MyVec vec_from(void* data, size_t element_size, size_t data_size) {
    MyVec self = vec_new_with_capacity(element_size, (data_size > DEFAULT_CAPACITY) ? data_size : DEFAULT_CAPACITY);

    if (data == NULL || self == NULL) {
        return NULL;                    // if self->data is not malloced self will be NULL
    }

    size_t byte_size = data_size;

    size_t large_chunks_count = byte_size / LARGE_CHUNK_SIZE;
    size_t small_chunks_count = byte_size % LARGE_CHUNK_SIZE;

    uint64_t* src_large_ptr = (uint64_t*)data;
    uint64_t* dst_large_ptr = (uint64_t*)self->data;

    for (size_t i = 0; i < large_chunks_count; i++) {
        dst_large_ptr[i] = src_large_ptr[i];
        self->size += LARGE_CHUNK_SIZE;
    }

    uint8_t* src_small_ptr = (uint8_t*)(src_large_ptr + large_chunks_count);
    uint8_t* dst_small_ptr = (uint8_t*)(dst_large_ptr + large_chunks_count);

    for (size_t i = 0; i < small_chunks_count; i++) {
        dst_small_ptr[i] = src_small_ptr[i];
        self->size += 1;
    }

    return self;
}

size_t len(MyVec self) {
    return self->size / self->element_size;
}

int resize(MyVec self, size_t new_size) {
    if (self == NULL) {
        return 0;
    }

    uint8_t* reallocated = realloc(self->data, new_size);
    if (reallocated == NULL) {
        return 0;
    }

    self->capacity = new_size;
    self->data = reallocated;
    return 1;
}

void push(MyVec self, void* element) {
    if (self == NULL || element == NULL) {
        return;
    }
    
    if (self->size == self->capacity) {
        if (!resize(self, self->capacity * 2 * self->element_size)) {
            return;
        }
    }

    uint8_t* bytes = (uint8_t*)element;

    for (int i = 0; i < self->element_size; i++) {
        self->data[self->size++] = *bytes++;
    }
}

uint8_t* get(MyVec self, size_t idx) {
    if (self == NULL || self->data == NULL || idx >= self->size || idx < 0) {
        return NULL;
    }

    return &self->data[idx * self->element_size];
}

uint8_t* pop(MyVec self) {
    if (self == NULL || self->data == NULL || self->size == 0) {
        return NULL;
    }

    self->size -= self->element_size;
    return &self->data[self->size];
}

int clear(MyVec self) {
    if (self == NULL || self->data == NULL) {
        return 0;
    }

    self->size = 0;
    return 1;
}

int append(MyVec self, MyVec source) {
    if (self == NULL || self->data == NULL || source == NULL || source->data == NULL || self->element_size != source->element_size) {
        return 0;
    }

    if (self->size + source->size >= self->capacity) {
        if (!resize(self, (self->capacity + source->size) * self->element_size)) {
            return 0;
        }
    }

    size_t large_chunks_count = source->size / LARGE_CHUNK_SIZE;
    size_t small_chunks_count = source->size % LARGE_CHUNK_SIZE;

    uint64_t* src_large_ptr = (uint64_t*)source->data;
    uint64_t* dst_large_ptr = (uint64_t*)(self->data + self->size);

    for (size_t i = 0; i < large_chunks_count; i++) {
        dst_large_ptr[i] = src_large_ptr[i];
        self->size += LARGE_CHUNK_SIZE;
    }

    uint8_t* src_small_ptr = (uint8_t*)(src_large_ptr + large_chunks_count);
    uint8_t* dst_small_ptr = (uint8_t*)(dst_large_ptr + large_chunks_count);

    for (size_t i = 0; i < small_chunks_count; i++) {
        dst_small_ptr[i] = src_small_ptr[i];
        self->size += 1;
    }
    
    return 1;
}

void destroy(MyVec self) {
    if (self == NULL) {
        return;
    }

    if (self->data != NULL) {
        free(self->data);
        self->data = NULL;
    }

    free(self);
    self = NULL;
}

/*##########################################################
#                 sorting implementations                  #
##########################################################*/

void sort_unsigned_8(uint8_t* data, size_t left, size_t right) {
    if (left >= right) {
        return;
    }

    size_t pivot_idx = (right - left) / 2 + left;
    uint8_t pivot = data[pivot_idx];

    data[pivot_idx] = data[right];
    data[right] = pivot;
    size_t lower_idx = left;

    for (size_t i = left; i < right; i++) {
        if (data[i] <= pivot) {
            uint8_t item  = data[i];
            
            data[i] = data[lower_idx];
            data[lower_idx] = item;

            lower_idx += 1;
        }
    }

    uint8_t lower_num = data[lower_idx];
    data[lower_idx] = data[right];
    data[right] = lower_num;

    sort_unsigned_8(data, left, (lower_idx > 0) ? lower_idx - 1 : 0);
    sort_unsigned_8(data, lower_idx + 1, right);
}

void sort_unsigned_16(uint16_t* data, size_t left, size_t right) {
    if (left >= right) {
        return;
    }

    size_t pivot_idx = (right - left) / 2 + left;
    uint16_t pivot = data[pivot_idx];

    data[pivot_idx] = data[right];
    data[right] = pivot;
    size_t lower_idx = left;

    for (size_t i = left; i < right; i++) {
        if (data[i] <= pivot) {
            uint16_t item  = data[i];
            
            data[i] = data[lower_idx];
            data[lower_idx] = item;

            lower_idx += 1;
        }
    }

    uint16_t lower_num = data[lower_idx];
    data[lower_idx] = data[right];
    data[right] = lower_num;

    sort_unsigned_16(data, left, (lower_idx > 0) ? lower_idx - 1 : 0);
    sort_unsigned_16(data, lower_idx + 1, right);
}

void sort_unsigned_32(uint32_t* data, size_t left, size_t right) {
    if (left >= right) {
        return;
    }

    size_t pivot_idx = (right - left) / 2 + left;
    uint32_t pivot = data[pivot_idx];

    data[pivot_idx] = data[right];
    data[right] = pivot;
    size_t lower_idx = left;

    for (size_t i = left; i < right; i++) {
        if (data[i] <= pivot) {
            uint32_t item  = data[i];
            
            data[i] = data[lower_idx];
            data[lower_idx] = item;

            lower_idx += 1;
        }
    }

    uint32_t lower_num = data[lower_idx];
    data[lower_idx] = data[right];
    data[right] = lower_num;

    sort_unsigned_32(data, left, (lower_idx > 0) ? lower_idx - 1 : 0);
    sort_unsigned_32(data, lower_idx + 1, right);
}

void sort_unsigned_64(uint64_t* data, size_t left, size_t right) {
    if (left >= right) {
        return;
    }

    size_t pivot_idx = (right - left) / 2 + left;
    uint64_t pivot = data[pivot_idx];

    data[pivot_idx] = data[right];
    data[right] = pivot;
    size_t lower_idx = left;

    for (size_t i = left; i < right; i++) {
        if (data[i] <= pivot) {
            uint64_t item  = data[i];
            
            data[i] = data[lower_idx];
            data[lower_idx] = item;

            lower_idx += 1;
        }
    }

    uint64_t lower_num = data[lower_idx];
    data[lower_idx] = data[right];
    data[right] = lower_num;

    sort_unsigned_64(data, left, (lower_idx > 0) ? lower_idx - 1 : 0);
    sort_unsigned_64(data, lower_idx + 1, right);
}

void sort_signed_8(int8_t* data, size_t left, size_t right) {
    if (left >= right) {
        return;
    }

    size_t pivot_idx = (right - left) / 2 + left;
    int8_t pivot = data[pivot_idx];

    data[pivot_idx] = data[right];
    data[right] = pivot;
    size_t lower_idx = left;

    for (size_t i = left; i < right; i++) {
        if (data[i] <= pivot) {
            int8_t item  = data[i];
            
            data[i] = data[lower_idx];
            data[lower_idx] = item;

            lower_idx += 1;
        }
    }

    int8_t lower_num = data[lower_idx];
    data[lower_idx] = data[right];
    data[right] = lower_num;

    sort_signed_8(data, left, (lower_idx > 0) ? lower_idx - 1 : 0);
    sort_signed_8(data, lower_idx + 1, right);
}

void sort_signed_16(int16_t* data, size_t left, size_t right) {
    if (left >= right) {
        return;
    }

    size_t pivot_idx = (right - left) / 2 + left;
    int16_t pivot = data[pivot_idx];

    data[pivot_idx] = data[right];
    data[right] = pivot;
    size_t lower_idx = left;

    for (size_t i = left; i < right; i++) {
        if (data[i] <= pivot) {
            int16_t item  = data[i];
            
            data[i] = data[lower_idx];
            data[lower_idx] = item;

            lower_idx += 1;
        }
    }

    int16_t lower_num = data[lower_idx];
    data[lower_idx] = data[right];
    data[right] = lower_num;

    sort_signed_16(data, left, (lower_idx > 0) ? lower_idx - 1 : 0);
    sort_signed_16(data, lower_idx + 1, right);
}

void sort_signed_32(int32_t* data, size_t left, size_t right) {
    if (left >= right) {
        return;
    }

    size_t pivot_idx = (right - left) / 2 + left;
    int32_t pivot = data[pivot_idx];

    data[pivot_idx] = data[right];
    data[right] = pivot;
    size_t lower_idx = left;

    for (size_t i = left; i < right; i++) {
        if (data[i] <= pivot) {
            int32_t item  = data[i];
            
            data[i] = data[lower_idx];
            data[lower_idx] = item;

            lower_idx += 1;
        }
    }

    int32_t lower_num = data[lower_idx];
    data[lower_idx] = data[right];
    data[right] = lower_num;

    sort_signed_32(data, left, (lower_idx > 0) ? lower_idx - 1 : 0);
    sort_signed_32(data, lower_idx + 1, right);
}

void sort_signed_64(int64_t* data, size_t left, size_t right) {
    if (left >= right) {
        return;
    }

    size_t pivot_idx = (right - left) / 2 + left;
    int64_t pivot = data[pivot_idx];

    data[pivot_idx] = data[right];
    data[right] = pivot;
    size_t lower_idx = left;

    for (size_t i = left; i < right; i++) {
        if (data[i] <= pivot) {
            int64_t item  = data[i];
            
            data[i] = data[lower_idx];
            data[lower_idx] = item;

            lower_idx += 1;
        }
    }

    int64_t lower_num = data[lower_idx];
    data[lower_idx] = data[right];
    data[right] = lower_num;

    sort_signed_64(data, left, (lower_idx > 0) ? lower_idx - 1 : 0);
    sort_signed_64(data, lower_idx + 1, right);
}

void sort(MyVec self, int is_signed) {
    switch (self->element_size) {
        case SIZE_8:
            if (is_signed) {
                sort_signed_8((int8_t*)self->data, 0, len(self) - 1);
            } else {
                sort_unsigned_8(self->data, 0, len(self) - 1);
            }
            break;
        case SIZE_16:
            if (is_signed) {
                sort_signed_16((int16_t*)self->data, 0, len(self) - 1);
            } else {
                sort_unsigned_16((uint16_t*)self->data, 0, len(self) - 1);
            }
            break;
        case SIZE_32:
            if (is_signed) {
                sort_signed_32((int32_t*)self->data, 0, len(self) - 1);
            } else {
                sort_unsigned_32((uint32_t*)self->data, 0, len(self) - 1);
            }
            break;
        case SIZE_64:
            if (is_signed) {
                sort_signed_64((int64_t*)self->data, 0, len(self) - 1);
            } else {
                sort_unsigned_64((uint64_t*)self->data, 0, len(self) - 1);
            }
            break;
    }
}