#include <gtest/gtest.h>
#include "memallocator.h"
#include <string.h>

TEST(MEMINIT, meminit)
{
    void* space = malloc(10000);
    if(NULL == space)
        printf("Memory is not given");
    meminit(space, 10000);

    EXPECT_EQ((*(descript*)space).block_size, 10000 - sizeof(descript) - 4);
    EXPECT_EQ((*(descript*)space).next, nullptr);
    EXPECT_EQ((*(descript*)space).prev, nullptr);

    meminit_finish();
}

TEST(FIRST_SUIT, first_suit)
{
    void* space = malloc(10000);
    if(NULL == space)
        printf("Memory is not given");
    meminit(space, 10000);

    descript* t1 = first_suit(1234);
    EXPECT_EQ(space, t1);

    memalloc(1234);
    descript* t2 = first_suit(1234);
    EXPECT_EQ(space, (char*)t2 - 1234 - 28);

    meminit_finish();
}

TEST(MEM_ALLOC, memalloc)
{
    void* space = malloc(10000);
    if(NULL == space)
        printf("Memory is not given");
    meminit(space, 10000);

    void* t1 = memalloc(50);
    EXPECT_EQ(space, (char*)t1 - sizeof(descript));

    void* t2 = memalloc(1234);
    EXPECT_EQ(t1, (char*)t2 - sizeof(descript) - 4 - 50);

    void* t3 = memalloc(123);
    mem_free(t2);

    void* t4 = memalloc(20);
    //  must go to the place, where t2 was before free
    EXPECT_EQ(t1, (char*)t4 - sizeof(descript) - 4 - 50);

    mem_free(t4);
    // enough place for memory, but not enough for even descriptor more
    void* t5 = memalloc(1220);
    EXPECT_EQ(t1, (char*)t5 - sizeof(descript) - 4 - 50);
    // the rest of memory (14 bytes) should not be left
    EXPECT_EQ(1234, ((descript*)((char*)t5 - sizeof(descript)))->block_size);
    //  => this one must go after t3
    void* t6 = memalloc(456);
    EXPECT_EQ(t3, (char*)t6 - sizeof(descript) - 4 - 123);

    mem_free(t5);
    //  free 1234 bytes must be skipped while searching for free space
    void* t7 = memalloc(1500);
    EXPECT_EQ(t6, (char*)t7 - sizeof(descript) - 4 - 456);

    meminit_finish();
}

TEST(MEMFREE, memfree)
{
    void* space = malloc(10000);
    if(NULL == space)
        printf("Memory is not given");
    mem_init(space, 10000);

    void* t1 = memalloc(16);
    void* t2 = memalloc(32);
    void* t3 = memalloc(48);
    void* t4 = memalloc(64);
    void* t5 = memalloc(80);

    descript* t3_1 = (descript*)((char*)t3 - sizeof(descript));
    EXPECT_EQ(*(int*)((char*)t3_1 + sizeof(descript) + (*t3_1).block_size), -48);
    mem_free(t3);
    EXPECT_EQ(*(int*)((char*)t3_1 + sizeof(descript) + (*t3_1).block_size), 48);

    mem_free(t4);  //  check if t4 unites with t3 (left)
    //  check block size
    EXPECT_EQ((*t3_1).block_size, 48 + sizeof(descript) + 4 + 64);
    //  check if is_free changed
    EXPECT_EQ(*(int*)((char*)t3_1 + sizeof(descript) + (*t3_1).block_size), 48 + sizeof(descript) + 4 + 64);

    mem_free(t2);  //  check if t2 unites with t3 (right)
    descript* t2_1 = (descript*)((char*)t2 - sizeof(descript));
    //  check block size
    EXPECT_EQ((*t2_1).block_size, 32 + sizeof(descript) + 4 + 48 + sizeof(descript) + 4 + 64);
    //  check if is_free changed
    EXPECT_EQ(*(int*)((char*)t2_1 + sizeof(descript) + (*t2_1).block_size), 32 + sizeof(descript) + 4 + 48 + sizeof(descript) + 4 + 64);

    meminit_finish();
}

