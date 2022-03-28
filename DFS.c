#include "DFS.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

list* create_list(int vertex_total_amount)
{
    list* head = (list*)malloc(sizeof(list)*vertex_total_amount);
    if(NULL == head)
    {
        printf("Memory is not given");
        return NULL;
    }
    for(int i = 0; i < vertex_total_amount; ++i)
    {
        head[i].vertex_sign = i;
        head[i].next_vertexes = NULL;
        head[i].next_vertexes_amount = 0;
    }
    return head;
}

void free_list(list* head, int vertex_total_amount)
{
    for(int i = 0; i < vertex_total_amount; ++i)
    {
        free(head[i].next_vertexes);
    }
    free(head);
}

void add_vertex(list* head, int place, int vertex)
{
    if(NULL == head[place].next_vertexes)
    {
        head[place].next_vertexes = (int*)malloc(sizeof(int) * 1024);
        if (head[place] == NULL) {
            exit(1);
        }
        head[place].next_vertexes[0] = vertex;
        head[place].next_vertexes_amount++;
    }
    else
    {
        int* buff = NULL;
        if(head[place].next_vertexes_amount % 1024 == 0)
        {
            buff = (int *)realloc(head[place].next_vertexes, sizeof(int) * (head[place].next_vertexes_amount + 1024));
            if(NULL == buff)
                exit(2);
            head[place].next_vertexes = buff;
        }
        head[place].next_vertexes[head[place].next_vertexes_amount] = vertex;
        head[place].next_vertexes_amount++;
    }

}

void input(list* head, int vertex_total_amount)
{
    int vertex;
    char space;
    for (int i = 0; i < vertex_total_amount; i++)
    {
        scanf("%d%c", &vertex, &space);
        head[i].vertex_sign = vertex;
        while (space == ' ') {
            scanf("%d%c", &vertex, &space);
            add_vertex(head, i, vertex);
            add_vertex(head, vertex, i);  //  directed graph to the undirected
        }
    }
}

void DFS(list* head, int vertex_total_amount, int vertex, int* sequence, int* hits_in_sequence)
{
    for(int i = 0; i < head[vertex].next_vertexes_amount; ++i)
    {
        int new = head[vertex].next_vertexes[i];

        for(int k = 0; k < *hits_in_sequence; ++k)
        {
            if(head[vertex].next_vertexes[i] == sequence[k])
            {
                new = 0;
            }
        }

        if(new != 0)
        {
            sequence[*hits_in_sequence] = new;
            (*hits_in_sequence)++;
            DFS(head, vertex_total_amount, new, sequence, hits_in_sequence);
        }
    }
}