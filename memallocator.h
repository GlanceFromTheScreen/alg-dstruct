#ifndef INC_2_HLAMKINEVGENIY_B11_MEMALLOCATOR_H
#define INC_2_HLAMKINEVGENIY_B11_MEMALLOCATOR_H

typedef struct descript{
    int block_size;  //  bez ucheta descriptora
    struct descript* next;
    struct descript* prev;
} descript;

//  creating the memory
void* mem_init(void* pMemory, int size);

//  function to see results (for programmer)
void check_head();

//  finds first suitable place fo required memory
descript* first_suit(int size);

//  malloc() analog
void* memalloc(int size);

//  writes the last block of descriptor after the data
void put_is_free(descript* pointer, int size, int b);

//  checks if the next block free
int check_is_free(descript* pointer);

//  free() analog
void mem_free(void* p);

//  cleans the mem_init actions, so next mem_init would work correct
void mem_init_finish();

#endif //INC_2_HLAMKINEVGENIY_B11_MEMALLOCATOR_H
