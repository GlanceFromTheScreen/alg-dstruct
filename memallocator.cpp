#include <stdio.h>
#include "memallocator.h"


typedef struct descript
{
    struct descript* next;
    struct descript* prev;
    int block_size;
    char is_free;
} descript;

descript* s = nullptr;
int full_size = 0;

int meminit(void* pMemory, int size) {
    if (s != NULL || size < sizeof(descript) || pMemory == NULL)
        return 1;

    s = (descript*)pMemory;

    s->block_size = size - sizeof(descript);
    s->next = NULL;
    s->prev = NULL;
    s->is_free = 1;

    full_size = size;

    return 0;
}

void check_head()
{
    if(s != nullptr)
    {
        printf("Own adress: %p\n", s);
        printf("Block size: %d\n", (*s).block_size);
        printf("Next: %p\n", (*s).next);
        printf("Previous: %p\n", (*s).prev);
        printf("S Is Free: %c\n", (*s).is_free);
        printf("\n");
    }
    else
        printf("--->S IS NULL<---");

}

descript* first_suit(int size)
{
    if (s == NULL)
        return NULL;

    descript* tmp = s;

    while (tmp != NULL)
    {
        if (tmp->is_free == 1 && tmp->block_size >= size)
            break;
        tmp = tmp->next;
    }

    return tmp;
}

void* memalloc(int size) {
    if (s == NULL)
        return NULL;

    descript* found_block = first_suit(size);

    if (found_block == NULL)
        return NULL;

    if (s == NULL || found_block == NULL || found_block->is_free == 0)
        return NULL;

    if (found_block->block_size - size > sizeof(descript))
    {
        descript* new_block = (descript*)((char*)(void*)((descript*)found_block + 1) + size);

        new_block->block_size = found_block->block_size - size - sizeof(descript);
        new_block->is_free = 1;
        new_block->next = found_block->next;
        new_block->prev = found_block;

        if (found_block->next != NULL)
            found_block->next->prev = new_block;

        found_block->next = new_block;
    }

    found_block->block_size = size;
    found_block->is_free = 0;

    return (void*)((descript*)found_block + 1);
}

void memfree(void* p)
{
    if (p == NULL)
        return;

    descript* free_block = (descript*)p - 1;

    if (free_block == NULL || free_block->is_free == 1)
        return;

    descript* buffer = free_block;
    buffer->is_free = 1;

    if (free_block->prev != NULL)
    {
        if (free_block->prev->is_free == 1)
        {
            int new_size = 0;

            if (free_block->next != NULL)
                new_size = (char*)free_block->next - (char*)(free_block->prev + 1);
            else
                new_size = (char*)(s + 1) + full_size - (char*)(free_block->prev + 1);

            free_block->prev->next = free_block->next;

            if (free_block->next != NULL)
                free_block->next->prev = free_block->prev;

            free_block->prev->block_size = new_size;
            buffer = free_block->prev;
        }
    }

    if (buffer->next != NULL)
    {
        if (buffer->next->is_free == 1)
        {
            int new_size = 0;

            if (buffer->next->next != NULL)
                new_size = (char*)buffer->next->next - (char*)(buffer + 1);
            else
                new_size = (char*)(s + 1) + full_size - (char*)(buffer + 1);

            if (buffer->next->next != NULL)
                buffer->next->next->prev = buffer;

            buffer->next = buffer->next->next;
            buffer->block_size = new_size;
        }
    }

    if (buffer->next != NULL)
    {
        if (((char*)(buffer + 1) + buffer->block_size) != (char*)buffer->next)
        {
            void* leaked_ptr = (void*)((char*)(buffer + 1) + buffer->block_size);
            int size = (char*)buffer->next - (char*)leaked_ptr;

            buffer->block_size += size;
        }
    }
    else
    if (((char*)s + full_size) != ((char*)(buffer + 1) + buffer->block_size))
    {
        void* leaked_ptr = (void*)((char*)(buffer + 1) + buffer->block_size);
        int size = (char*)s + full_size - (char*)leaked_ptr;

        buffer->block_size += size;
    }
}

void memdone()
{
    int size = 0;

    if (s == NULL)
        return;

    descript* block = s;

    while (block != NULL)
    {
        if (block->is_free == 1)
            size += block->block_size;

        block = block->next;
    }

    size += sizeof(descript);

    if (size < full_size)
        fprintf(stderr, "Memory leak: \n\texpected %d\n\tcurrent %d\n", full_size, size);
}

void meminit_finish()
{
    s = NULL;
}

int memgetminimumsize()
{
    return sizeof(descript);
}

int memgetblocksize()
{
    return sizeof(descript);
}