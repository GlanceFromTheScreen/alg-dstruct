#include <stdio.h>
#include "DFS.h"
#include <stdlib.h>

int main()
{
    int vertex_total_amount = 0;
    scanf("%d", &vertex_total_amount);

    list* D = create_list(vertex_total_amount);
    input(D, vertex_total_amount);

    int* arr = malloc(sizeof(int)*vertex_total_amount);
    arr[0] = 0;
    int r = 1;

    DFS(D, vertex_total_amount, 0, arr, &r);

    for(int i = 0; i < r; ++i)
        printf("%d ", arr[i]);

    return 0;
}
