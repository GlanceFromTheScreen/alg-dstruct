#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 1000000 // table
#define len 100000  //  str

typedef struct Table
{
    char* pointer;
    int key;  //  1 - removed pos
}Table;

Table* CreateTable()
{
    Table* t = malloc(N*sizeof(Table));
    if(NULL == t)
        return NULL;
    for (int i = 0; i < N; i++)
    {
        t[i].pointer = NULL;
        t[i].key = 0;
    }
    return t;
}

int HashFunction(char* str, int pass)
{
    int h = 21;
    int hash_1 = 0;
    for (int i = 0; i < strlen(str); i++)
        hash_1 = h * hash_1 + str[i];
    hash_1 = hash_1 % N;
    return (int)(hash_1 + pass) % N;
}


void AddToTable(Table* table, char* str)
{
    int j = 0;
    int pos = HashFunction(str, j);
    int cmp = 1;
    while(table[pos].pointer != NULL && cmp != 0 && pos != N)
    {
        cmp = strcmp(table[pos].pointer, str);
        j++;
        pos = HashFunction(str, j);
    }
    if(cmp == 0 || pos == N)
        return;

    table[pos].pointer = malloc((strlen(str) + 1)*sizeof(char));
    strcpy(table[pos].pointer, str);
    table[pos].key = 0;
}

void Search(Table* table, char* str)
{
    int j = 0;
    int pos = HashFunction(str, j);
    int cmp = 1;
    while(table[pos].pointer != NULL && cmp != 0 && pos != N)
    {
        if(table[pos].key == 0)
            cmp = strcmp(table[pos].pointer, str);
        j++;
        pos = HashFunction(str, j);
    }
    if(cmp == 0)
        printf("yes\n");
    else
        printf("no\n");

}



void DeleteFromTable(Table* table, char* str)
{
    int j = 0;
    int pos = HashFunction(str, j);
    int cmp = 1;
    int cop = pos;
    while(table[pos].pointer != NULL && cmp != 0 && pos != N)
    {
        cmp = strcmp(table[pos].pointer, str);
        j++;
        cop = pos;
        pos = HashFunction(str, j);
    }
    if(cmp == 0)
    {
        free(table[cop].pointer);
        table[cop].key = 1;
    }
}

int main()
{
    Table* t1 = CreateTable();
    char command;
    char value[len];
    while (scanf("%c", &command) > 0)
    {
        if (command == 'a')
        {
            scanf("%s", &value);
            AddToTable(t1,value);
        }
        if (command == 'f')
        {
            scanf("%s", &value);
            Search(t1,value);
        }
        if (command == 'r')
        {
            scanf("%s", &value);
            DeleteFromTable(t1, value);
        }
    }

    return 0;
}
