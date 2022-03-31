#ifndef INC_2_HLAMKINEVGENIY_B11_MEMALLOCATOR_H
#define INC_2_HLAMKINEVGENIY_B11_MEMALLOCATOR_H


//  creating the memory
int meminit(void* pMemory, int size);

//  function to see results (for programmer)
void check_head();

//  finds first suitable place fo required memory
descript* first_suit(int size);

//  malloc() analog
void* memalloc(int size);

//  free() analog
void memfree(void* p);

//  cleans the mem_init actions, so next mem_init would work correct
void meminit_finish();

int memgetblocksize();

int memgetminimumsize();

void memdone();

#endif //INC_2_HLAMKINEVGENIY_B11_MEMALLOCATOR_H
