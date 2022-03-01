#ifndef MAIN_C_SORT_H
#define MAIN_C_SORT_H

typedef struct Node
{
    char* value;
    struct Node *next;
} list;

//  CREATING THE LIST WITH 1 NODE GIVEN
list* create(char* data);

void list_print(list* head);

void remove_list(list* head);

//  CHANGE OF N AND N+1 NODES
void change_two_nodes(int n, list* head);

void add_element_end(char* data, list *head);

//  COMPARES TWO ELEMENTS
int compare(char* first, char* second);

//  SORTING FUNCTION
list* sort(list* head);

//  INSERTS THE ELEMENT FROM M POSISION TO N POSISION
void insert(int n, int m, list* head);

list* add_node(char* data, int n, list* head);

#endif //MAIN_C_SORT_H
