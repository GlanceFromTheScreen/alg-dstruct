#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct tree
{
    char* word;
    int size;
    struct tree* left;  //  strings compare. Less one goes to the left
    struct tree* right;
} tree;

// Creating the tree
tree* CreateTree(char* word)
{
    tree* head = (tree*)malloc(sizeof(tree));
    if (head == NULL)
    {
        printf("Memory is not given");
        return NULL;
    }
    head->word = word;
    head->size = 0;
    head->left = NULL;
    head->right = NULL;
    return head;
}

// Adding new verdex as char*
void AddVertex(tree* head, char* word)
{
    tree* new_vertex = CreateTree(word);

    if (new_vertex == NULL)
        return;
    if (head == NULL)
    {
        head = new_vertex;
        return;
    }
    tree* tmp = head;

    while (tmp != NULL) {
        if (strcmp(new_vertex->word, tmp->word) < 0)
        {
            if (tmp->left == NULL)
            {
                tmp->left = new_vertex;
                return;
            } else {
                tmp = tmp->left;
            }
        } else if (strcmp(new_vertex->word, tmp->word) == 0) {
            free(new_vertex);
            return;
        } else {
            if (tmp->right == NULL)
            {
                tmp->right = new_vertex;
                return;
            } else {
                tmp = tmp->right;
            }
        }
    }
}

// Check weather vertex in the tree
int Search(tree* head, char* word)
{
    tree* tmp = head;
    while (tmp != NULL) {
        if (strcmp(word, tmp->word) < 0)
        {
            tmp = tmp->left;
        } else {
            if (*word == *tmp->word)
                return 1;
            else {
                tmp = tmp->right;
            }
        }
    }
    return 0;
}

// Adding new vertex as already existed tree
void AddVertex_2(tree* head, tree* new_vertex)
{
    if (new_vertex == NULL)
        return;
    if (head == NULL)
    {
        head = new_vertex;
        return;
    }
    tree* tmp = head;

    while (tmp != NULL) {
        if (strcmp(new_vertex->word, tmp->word) < 0)
        {
            if (tmp->left == NULL)
            {
                tmp->left = new_vertex;
                return;
            } else {
                tmp = tmp->left;
            }
        } else if (strcmp(new_vertex->word, tmp->word) == 0) {
            free(new_vertex);
            return;
        } else {
            if (tmp->right == NULL)
            {
                tmp->right = new_vertex;
                return;
            } else {
                tmp = tmp->right;
            }
        }
    }
}

// Deliting the tree
void DeleteTree(tree* head, char* word)
{
    if (head == NULL)
        return;

    tree* tmp = head;
    tree* buff = tmp;
    bool flag = 1;

    while (tmp != NULL) {
        if (strcmp(word, tmp->word) < 0)
        {
            buff = tmp;
            flag = 1;
            tmp = tmp->left;
        } else {
            if (strcmp(word, tmp->word) == 0)
            {
                if (tmp == head)
                {
                    if (head->right != NULL)
                    {
                        tmp = head->left;
                        head = head->right;
                        AddVertex_2(head, tmp);
                    } else {
                        printf("\n%p\n", head->left);
                        head = &(*head->left);
                        printf("%p\n", head);
                        head->size = 9;
                    }
                    return;
                }

                if (flag) {
                    buff->left = NULL;
                } else {
                    buff->right = NULL;
                }
                AddVertex_2(head, tmp->left);
                AddVertex_2(head, tmp->right);
                free(tmp);
                return;
            } else {
                buff = tmp;
                flag = 0;
                tmp = tmp->right;
            }
        }
    }
}

// Finding the width of the subtree
int FindWidth(tree* head) {
    if (head == NULL) {
        return 0;
    }
    head->size = strlen(head->word) + FindWidth(head->left) + FindWidth(head->right);
    return head->size;
}

// Printing the tree
void TreePrint(tree* head, int n) {
    if (head != NULL)
    {
        TreePrint(head->left, n + 2);
        for (int i = 0; i < n; i++)
            printf(" ");
        printf("%s\n", head->word);
        for (int i = 0; i < n; i++)
            printf(" ");
        FindWidth(head);
        printf("%i\n", head->size);
        TreePrint(head->right, n+2);
    }
    else
        return;
}

int main()
{

    tree* head = CreateTree("5-first");
    AddVertex(head, "3-second");
    AddVertex(head, "4-third");
    TreePrint(head, 0);

    char v[2];
    char** v2[100];  //   array of pointers to char*
    int g = 0;
    while(v[0] != 113)  //  q
    {
        scanf("%s", &v);
        g++;

        if(v[0] != 112 && v[0] != 113)  //  not p and not q
        {
            scanf("%s", v2+g);
        }
        if(v[0] == 97)  //  a
        {
            AddVertex(head, v2+g);
        }
        if(v[0] == 112)  //  p
        {
            TreePrint(head, 0);
        }
        if(v[0] == 114)  //  r
        {
            DeleteTree(head, v2+g);
        }
        if(v[0] == 102)  //  f
        {
            if(Search(head, v2+g) == 1)
                printf("yes");
            else
                printf("no");
        }
    }

    return 0;
}