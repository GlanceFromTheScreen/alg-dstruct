#include "memallocator.h"
#include <stdio.h>

descript* s = NULL;  //  pointer to our memory, that can be given to user
int absolute_size;
int free_block;

void put_is_free(descript* pointer, int size, int b)
{
    int* f = (int*)((char*)pointer + sizeof(descript) + size);
    *f = b;
}

int check_is_free(descript* pointer)
{
    return *(int*)((char*)pointer + sizeof(descript) + (*pointer).block_size);
}

void* mem_init(void* pMemory, int size)
{
    if (s != NULL || size < sizeof(descript) || pMemory == NULL)
        return NULL;  //  not succeed

    s = (descript*)pMemory;

    (*s).block_size = size - sizeof(descript) - 4;
    (*s).next = NULL;
    (*s).prev = NULL;
    put_is_free(s, size - sizeof(descript) - 4, (*s).block_size);
    return 0;
}

void mem_init_finish()
{
    s = NULL;
}

void check_head()
{
    if(s != NULL)
    {
        printf("Own adress: %p\n", s);
        printf("Block size: %d\n", (*s).block_size);
        printf("Next: %p\n", (*s).next);
        printf("Previous: %p\n", (*s).prev);
        printf("S Is Free: %d\n", check_is_free(s));
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
        if (check_is_free(tmp) >= 0 && (*tmp).block_size >= size)
            break;

        tmp = (*tmp).next;
    }
    return tmp;
}

void* memalloc(int size)
{

    descript* tmp = first_suit(size);

    if(tmp == NULL)
    {
        printf("TOO MUCH MEMORY REQUIRED\n\n");
        return NULL;
    }

    descript* check_s_tmp = tmp;

    int bs = (*tmp).block_size;

    if((*tmp).block_size - size > sizeof(descript) + 4)
    {
        descript* sn =  (*tmp).next;
        descript* sp =(*tmp).prev;

        (*tmp).block_size = size;
        put_is_free(tmp, size, (-1)*(*tmp).block_size);

        tmp = (descript*)((char*)tmp + size + sizeof(descript) + 4);
        (*tmp).block_size = bs - size - sizeof(descript) - 4;
        (*tmp).next = sn;
        (*tmp).prev = sp;

        put_is_free(tmp, (*tmp).block_size, (*tmp).block_size);

        if((*tmp).prev != NULL)
            (*(*tmp).prev).next = tmp;

        if((*tmp).next != NULL)
            (*(*tmp).next).prev = tmp;

        if(check_s_tmp == s)
            s = tmp;

    }

    else if((*tmp).block_size - size <= sizeof(descript) + 4)
    {
        (*tmp).block_size = bs;
        put_is_free(tmp, (*tmp).block_size, (-1)*(*tmp).block_size);

        if((*tmp).prev != NULL)
            (*(*tmp).prev).next = (*tmp).next;

        if((*tmp).next != NULL)
            (*(*tmp).next).prev = (*tmp).prev;

        tmp = tmp->next;
        if(check_s_tmp == s)
            s = tmp;
    }

    return (void*)((char*)check_s_tmp + sizeof(descript));  //  tmp - pointer to the descriptor. Returns pointer to the free space => p+size...
}

void mem_free(void* p)
{
    if (p == NULL)
        return;

    descript* free_block = (descript*)((char*)p - sizeof(descript));
    int left_block = *((int*)((char*)free_block - 4));
    descript* r = (descript*)(((char*)free_block + sizeof(descript) + 4 + (*free_block).block_size));
    int right_block = *(int*)((char*)r + sizeof(descript) + (*r).block_size);

    if(left_block < 0 && right_block < 0)
    {
        descript* H = s;
        s = free_block;
        (*s).next = H;
        if((*s).next != NULL)
            (*s).next->prev = s;
        put_is_free(s, (*s).block_size, (*s).block_size);
    }
    else
    {
        if(right_block >= 0)
        {
            (*free_block).block_size += (*r).block_size + sizeof(descript) + 4;
            if((*r).prev != NULL)
                (*r).prev->next = free_block;
            if((*r).next != NULL)
                (*r).next->prev = free_block;
            (*free_block).next = (*r).next;
            (*free_block).prev = (*r).prev;
            put_is_free(free_block, (*free_block).block_size,(*free_block).block_size);

            if(r == s)
            {
                s = free_block;
            }

        }
        if(left_block >= 0)
        {
            descript* L = (descript*)((char*)free_block - 4 - left_block - sizeof(descript));
            //printf("%d\n", (*L).block_size);
            (*L).block_size += sizeof(descript) + 4 + (*free_block).block_size;
            put_is_free(L, (*L).block_size,(*L).block_size);


            if(right_block >= 0)
            {
                (*L).next = (*L).next->next;
            }

            if(L == s)
            {
                s = L;
            }


        }

    }
}
