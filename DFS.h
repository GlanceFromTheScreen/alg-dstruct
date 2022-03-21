#ifndef INC_2_HLAMKINEVGENIY_C1_DFS_H
#define INC_2_HLAMKINEVGENIY_C1_DFS_H

typedef struct list
{
    int vertex_sign;
    int* next_vertexes;  //  link to the int array of next vertexes
    int next_vertexes_amount;
} list;

list* create_list(int vertex_total_amount);

void free_list(list* head, int vertex_total_amount);

void add_vertex(list* head, int place, int vertex);

void input(list* head, int vertex_total_amount);

void DFS(list* head, int vertex_total_amount, int vertex, int* sequence, int* hits_in_sequence);

#endif //INC_2_HLAMKINEVGENIY_C1_DFS_H
