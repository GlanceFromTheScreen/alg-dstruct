#include <stdio.h>
#include <stdlib.h>
#include "memallocator.h"
#include <time.h>
#include <windows.h>
#include <gtest/gtest.h>

using namespace testing;


#define MAX_ALLOC_SIZE 250
#define TEST_ITERATIONS 700

int TEST_MEM_INIT();

int main()
{
    InitGoogleTest();
    return RUN_ALL_TESTS();
}
