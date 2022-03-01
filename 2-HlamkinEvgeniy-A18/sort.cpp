#include "sort.h"
#include <stdio.h>
#include <stdlib.h>

list* create(char* data)
{
    list* tmp = (list*)malloc(sizeof(list));
    tmp -> value = data;
    tmp -> next = NULL;
    return(tmp);
}  //  max use

void list_print(list* head)
{
    while (head -> next != NULL)
    {
        puts(head->value);
        head = head -> next;
    }
    puts(head->value);
}  // max use

void add_element_end(char* data, list* head)
{
    list *tmp = (list*)malloc(sizeof(list));
    if (tmp) {
        tmp->value = data;
        tmp->next = NULL;

        list *p = head;

        while (p->next != NULL)
            p = p->next;

        p->next = tmp;
    } else perror("Malloc failed");
}  //  useful, but not nessesery

void remove_list(list* head)
{
    while (head != NULL)
    {
        list* p = head;
        head = head->next;
        free(p);
    }
    free(head);
}  //  may be used

void change_two_nodes(int n, list* head)  //  used in incert
{
    list* p = head;
    int count = 0;
    while (count < n - 1)
    {

        p = p -> next;
        count++;
    }

    if(n != 0) {
        list *buffer = p->next->next;
        p->next->next = p->next->next->next;
        (*buffer).next = p->next;
        p->next = buffer;
    }
    else{
        char* buffer = head->value;
        head->value = head->next->value;
        head->next->value = buffer;
    }

}

int compare(char* first, char* second)
{
    int i = 0;
    while(first[i] && second[i])
    {
        if(first[i] > second[i]){
            return 1;
        }
        else if(first[i] < second[i]){
            return -1;
        }
        i++;
    }
    if(first[i])
        return 1;
    else if(second[i])
        return -1;
    else
        return 0;

}  //  max use

list* sort(list* head)
{
    char char_tmp[] = "a\0";
    char char_tmp_2[] = "z\0";
    list* head_tmp = add_node(char_tmp, 0, head);
    add_element_end(char_tmp_2, head_tmp);
    list* p = head_tmp->next; // gradually goes to the .next
    list* q = head_tmp->next; // = head in every loop
    int i = 1;
    int j = 0;
    bool flag = false;
    p = p->next;
    while(p->next != NULL) {
        flag = false;
        while (j < i) {
            if(compare(p->value, q->value) == -1) {
                p = p->next;
                flag = true;
                insert(j+1, i+1, head_tmp);
                break;
            }
            j++;
            q = q->next;
        }
        if(flag == false){
            p = p->next;
        }

        q = head_tmp->next;
        i++;
        j = 0;
    }

    list* del1 = head_tmp;  //  deleting the last element
    while(del1->next->next != NULL)
    {
        del1 = del1->next;
    }
    del1->next = NULL;
    return head_tmp->next;
}  //  the purpose

void insert(int n, int m, list* head)  //  changing n and m (n better < m) nodes in head list
{
    list *p = head;
    list *q = head;
    int count = 0;
    int count2 = 0;

    if (m - n == 1)
        change_two_nodes(n, head);
    else if (n - m == 1)
        change_two_nodes(m, head);
    else {

        while (count < n - 1) {
            p = p->next;
            count++;
        }

        while (count2 < m - 1) {
            q = q->next;
            count2++;
        }

        if (n != 0 && m != 0) {
            list *buffer1 = q->next->next;
            q->next->next = p->next;
            p->next = q->next;
            q->next = buffer1;
        }

    }
}

list *add_node(char *data, int n, list *head) {
    list *p = head;
    int count = 0;
    while (count < n - 1 && p->next != NULL) {
        p = p->next;
        count++;
    }

    list *tmp = (list *) malloc(sizeof(list));
    if (tmp) {
        if (n != 0) {
            tmp->value = data;
            tmp->next = p->next;
            p->next = tmp;
            return p;
        } else {
            tmp->value = data;
            tmp->next = head;
            return tmp;
        }
    } else perror("Malloc failed");

}  //  used in sort

