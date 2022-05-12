#include <stdio.h>
#include <stdlib.h>

typedef struct Tree
{
    int size;
    int keys[3];  //  the 3rd one is a buffer
    struct Tree* child[4];  //  the 4th one is a buffer
    struct Tree* parent;
} Tree;

Tree* Divide(Tree* node);
Tree* CaseE(Tree* leaf);
Tree* CaseD(Tree* leaf);
Tree* LeafRec(Tree* leaf);

//  check if there is a key in node
int CheckKeyInNode(Tree* node, int key)
{
    for (int i = 0; i < node->size; ++i)
    {
        if (node->keys[i] == key)
        {
            return 1;
        }
    }

    return 0;
}

//  sort keys in one node
void SortKeys(Tree* node)
{
    if (node == NULL)
    {
        return;
    }

    if (node->size == 1)
    {
        return;
    }

    if (node->size == 2)
    {
        if (node->keys[0] > node->keys[1])
        {
            int r = node->keys[0];
            node->keys[0] = node->keys[1];
            node->keys[1] = r;
        }
    }

    if (node->size == 3)
    {
        if (node->keys[0] > node->keys[1])
        {
            int r = node->keys[0];
            node->keys[0] = node->keys[1];
            node->keys[1] = r;
        }

        if (node->keys[0] > node->keys[2])
        {
            int r = node->keys[0];
            node->keys[0] = node->keys[2];
            node->keys[2] = r;
        }

        if (node->keys[1] > node->keys[2])
        {
            int r = node->keys[1];
            node->keys[1] = node->keys[2];
            node->keys[2] = r;
        }
    }
}

//  insert the key in node only if there is enaugh place
void InsertKeyLight(Tree* node, int key)
{
    if (node == NULL)
    {
        return;
    }

    if (node->size >= 3)
    {
        return;
    }

    node->keys[node->size] = key;
    node->size++;
    SortKeys(node);
}

//  delete key only if node has not less then 2 keys
void DeleteKeyLight(Tree* node, int key)
{
    if (node == NULL)
    {
        return;
    }

    if (node->size >= 1 && node->keys[0] == key)
    {
        node->keys[0] = node->keys[1];
        node->keys[1] = node->keys[2];
        node->size--;
    }
    else if (node->size == 2 && node->keys[1] == key)
    {
        node->keys[1] = node->keys[2];
        node->size--;
    }
}

//  unions 2 nodes under 1 node
void TwoNodeUnion(Tree* node, Tree* first, Tree* second, int key)
{
    if (node == NULL)
    {
        return;
    }

    node->keys[0] = key;
    node->child[0] = first;
    node->child[1] = second;
    node->child[2] = NULL;
    node->child[3] = NULL;
    node->parent = NULL;
    node->size = 1;
}

//  check if the node is a leaf
int CheckOnLeaf(Tree* node)
{
    if (node == NULL)
    {
        return 0;
    }

    return (node->child[0] == NULL) && (node->child[1] == NULL) && (node->child[2] == NULL);
}

//  creates tree by key
Tree* TreeCreate(int key)
{
    Tree* node = (Tree*)malloc(sizeof(Tree));
    if (node == NULL)
    {
        return NULL;
    }

    node->child[0] = node->child[1] = node->child[2] = node->child[3] = NULL;
    node->keys[0] = key;
    node->keys[1] = node->keys[2] = 0;
    node->parent = NULL;
    node->size = 1;

    return node;
}

//  creates tree by parametrs
Tree* TreeByParametrs(int key, Tree* first, Tree* second, Tree* third, Tree* fourth, Tree* parent)
{
    Tree* node = (Tree*)malloc(sizeof(Tree));
    if (node == NULL)
    {
        return NULL;
    }

    node->child[0] = first;
    node->child[1] = second;
    node->child[2] = third;
    node->child[3] = fourth;
    node->keys[0] = key;
    node->keys[1] = node->keys[2] = 0;
    node->parent = parent;
    node->size = 1;

    return node;
}

//  insert in any condition (uses Divide) - recursion
Tree* Insert(Tree* node, int key)
{
    if (node == NULL)
    {
        return TreeCreate(key);
    }

    if (CheckKeyInNode(node, key))
    {
        return node;
    }

    if (CheckOnLeaf(node))
    {
        InsertKeyLight(node, key);
    }
    else if (key < node->keys[0])
    {
        Insert(node->child[0], key);
    }
    else if ((node->size == 1) || ((node->size == 2) && key < node->keys[1]))
    {
        Insert(node->child[1], key);
    }
    else
    {
        Insert(node->child[2], key);
    }

    return Divide(node);
}

//  separates node if insert
Tree* Divide(Tree* node)
{
    if (node == NULL)
    {
        return node;
    }

    if (node->size < 3)
    {
        return node;
    }

    //  case of 3 keys in 1 node:
    Tree* x = TreeByParametrs(node->keys[0], node->child[0], node->child[1], NULL, NULL, node->parent);
    Tree* y = TreeByParametrs(node->keys[2], node->child[2], node->child[3], NULL, NULL, node->parent);

    // remember parents:
    if (x->child[0])
    {
        x->child[0]->parent = x;
    }

    if (x->child[1])
    {
        x->child[1]->parent = x;
    }

    if (y->child[0])
    {
        y->child[0]->parent = y;
    }

    if (y->child[1])
    {
        y->child[1]->parent = y;
    }

    //  inserting the middle key to parent
    if (node->parent != NULL)
    {
        InsertKeyLight(node->parent, node->keys[1]);

        if (node->parent->child[0] == node)
        {
            node->parent->child[0] = NULL;
        }
        else if (node->parent->child[1] == node)
        {
            node->parent->child[1] = NULL;
        }
        else if (node->parent->child[2] == node)
        {
            node->parent->child[2] = NULL;
        }

        if (node->parent->child[0] == NULL) {
            node->parent->child[3] = node->parent->child[2];
            node->parent->child[2] = node->parent->child[1];
            node->parent->child[1] = y;
            node->parent->child[0] = x;
        }
        else if (node->parent->child[1] == NULL) {
            node->parent->child[3] = node->parent->child[2];
            node->parent->child[2] = y;
            node->parent->child[1] = x;
        }
        else {
            node->parent->child[3] = y;
            node->parent->child[2] = x;
        }

        Tree* tmp = node->parent;

        free(node);

        return tmp;
    }
    else
    {
        x->parent = node;
        y->parent = node;

        TwoNodeUnion(node, x, y, node->keys[1]);

        return node;
    }
}

//  find node
Tree* Find(Tree* node, int key)
{
    if (node == NULL)
    {
        return NULL;
    }

    if (CheckKeyInNode(node, key))
    {
        return node;
    }
    else if (key < node->keys[0])
    {
        return Find(node->child[0], key);
    }
    else if ((node->size == 2) && (key < node->keys[1]) || (node->size == 1))
    {
        return Find(node->child[1], key);
    }
    else if (node->size == 2)
    {
        return Find(node->child[2], key);
    }
}

//  find min (for Delete)
Tree* FindMin(Tree* node)
{
    if (node == NULL)
    {
        return NULL;
    }

    if (node->child[0] == NULL)
    {
        return node;
    }
    else
    {
        return FindMin(node->child[0]);
    }
}

//  delete prepare
Tree* Delete(Tree* node, int key)
{
    if (node == NULL)
    {
        return NULL;
    }

    Tree* item = Find(node, key);

    if (item == NULL)
    {
        return node;
    }

    Tree* minNode = NULL;
    if (item->keys[0] == key)
    {
        minNode = FindMin(item->child[1]);  //  middle
    }
    else
    {
        minNode = FindMin(item->child[2]);  //  right
    }

    //  change to leaf
    if (minNode != NULL)
    {
        int* z = (key == item->keys[0] ? &(item->keys[0]) : &(item->keys[1]));
        int r = *z;
        *z = minNode->keys[0];
        minNode->keys[0] = r;
        item = minNode;
    }

    DeleteKeyLight(item, key);

    return LeafRec(item);
}

//  recovers 2-3 tree structure (using Cases) - recursion
Tree* LeafRec(Tree* leaf)
{
    if (leaf == NULL)
    {
        return NULL;
    }

    if (leaf->size == 0 && leaf->parent == NULL)
    {
        free(leaf);
        return NULL;
    }

    if (leaf->size != 0) {
        if (leaf->parent)
        {
            return LeafRec(leaf->parent);
        }
        else
        {
            return leaf;
        }
    }

    Tree* parent = leaf->parent;
    if (parent->child[0]->size == 2 || parent->child[1]->size == 2 || parent->size == 2 || (parent->size == 2 && parent->child[2]->size == 2))
    {
        leaf = CaseD(leaf);
    }
    else
    {
        leaf = CaseE(leaf);
    }

    return LeafRec(leaf);
}

//  cases for delete
Tree* CaseD(Tree* leaf)
{
    if (leaf == NULL)
    {
        return leaf;
    }

    Tree* parent = leaf->parent;
    Tree* first = parent->child[0];
    Tree* second = parent->child[1];
    Tree* third = parent->child[2];

    if ((parent->size == 2) && (first->size < 2) && (second->size < 2) && (third->size < 2))
    {
        if (first == leaf)
        {
            parent->child[0] = parent->child[1];
            parent->child[1] = parent->child[2];
            parent->child[2] = NULL;
            InsertKeyLight(parent->child[0], parent->keys[0]);
            parent->child[0]->child[2] = parent->child[0]->child[1];
            parent->child[0]->child[1] = parent->child[0]->child[0];

            if (leaf->child[0] != NULL)
            {
                parent->child[0]->child[0] = leaf->child[0];
            }
            else if (leaf->child[1] != NULL)
            {
                parent->child[0]->child[0] = leaf->child[1];
            }

            if (parent->child[0]->child[0] != NULL)
            {
                parent->child[0]->child[0]->parent = parent->child[0];
            }

            DeleteKeyLight(parent, parent->keys[0]);

            if (first != NULL)
            {
                free(first);
            }
        }
        else if (second == leaf)
        {
            InsertKeyLight(first, parent->keys[0]);
            DeleteKeyLight(parent, parent->keys[0]);

            if (leaf->child[0] != NULL)
            {
                first->child[2] = leaf->child[0];
            }
            else if (leaf->child[1] != NULL)
            {
                first->child[2] = leaf->child[1];
            }

            if (first->child[2] != NULL)
            {
                first->child[2]->parent = first;
            }

            parent->child[1] = parent->child[2];
            parent->child[2] = NULL;

            if (second != NULL)
            {
                free(second);
            }
        }
        else if (third == leaf)
        {
            InsertKeyLight(second, parent->keys[1]);
            parent->child[2] = NULL;
            DeleteKeyLight(parent, parent->keys[1]);

            if (leaf->child[0] != NULL)
            {
                second->child[2] = leaf->child[0];
            }
            else if (leaf->child[1] != NULL)
            {
                second->child[2] = leaf->child[1];
            }

            if (second->child[2] != NULL)
            {
                second->child[2]->parent = second;
            }

            if (third != NULL)
            {
                free(third);
            }
        }
    }
    else if ((parent->size == 2) && ((first->size == 2) || (second->size == 2) || (third->size == 2)))
    {
        if (third == leaf)
        {
            if (leaf->child[0] != NULL)
            {
                leaf->child[1] = leaf->child[0];
                leaf->child[0] = NULL;
            }

            InsertKeyLight(leaf, parent->keys[1]);
            if (second->size == 2)
            {
                parent->keys[1] = second->keys[1];
                DeleteKeyLight(second, second->keys[1]);
                leaf->child[0] = second->child[2];
                second->child[2] = NULL;

                if (leaf->child[0] != NULL)
                {
                    leaf->child[0]->parent = leaf;
                }
            }
            else if (first->size == 2)
            {
                parent->keys[1] = second->keys[0];
                leaf->child[0] = second->child[1];
                second->child[1] = second->child[0];

                if (leaf->child[0] != NULL)
                {
                    leaf->child[0]->parent = leaf;
                }

                second->keys[0] = parent->keys[0];
                parent->keys[0] = first->keys[1];
                DeleteKeyLight(first, first->keys[1]);
                second->child[0] = first->child[2];

                if (second->child[0] != NULL)
                {
                    second->child[0]->parent = second;
                }

                first->child[2] = NULL;
            }
        }
        else if (second == leaf)
        {
            if (third->size == 2)
            {
                if (leaf->child[0] == NULL)
                {
                    leaf->child[0] = leaf->child[1];
                    leaf->child[1] = NULL;
                }

                InsertKeyLight(second, parent->keys[1]);
                parent->keys[1] = third->keys[0];
                DeleteKeyLight(third, third->keys[0]);
                second->child[1] = third->child[0];

                if (second->child[1] != NULL)
                {
                    second->child[1]->parent = second;
                }

                third->child[0] = third->child[1];
                third->child[1] = third->child[2];
                third->child[2] = NULL;
            }
            else if (first->size == 2)
            {
                if (leaf->child[1] == NULL)
                {
                    leaf->child[1] = leaf->child[0];
                    leaf->child[0] = NULL;
                }

                InsertKeyLight(second, parent->keys[0]);
                parent->keys[0] = first->keys[1];
                DeleteKeyLight(first, first->keys[1]);
                second->child[0] = first->child[2];

                if (second->child[0] != NULL)
                {
                    second->child[0]->parent = second;
                }

                first->child[2] = NULL;
            }
        }
        else if (first == leaf)
        {
            if (leaf->child[0] == NULL)
            {
                leaf->child[0] = leaf->child[1];
                leaf->child[1] = NULL;
            }

            InsertKeyLight(first, parent->keys[0]);

            if (second->size == 2)
            {
                parent->keys[0] = second->keys[0];
                DeleteKeyLight(second, second->keys[0]);
                first->child[1] = second->child[0];

                if (first->child[1] != NULL)
                {
                    first->child[1]->parent = first;
                }

                second->child[0] = second->child[1];
                second->child[1] = second->child[2];
                second->child[2] = NULL;
            }
            else if (third->size == 2)
            {
                parent->keys[0] = second->keys[0];
                second->keys[0] = parent->keys[1];
                parent->keys[1] = third->keys[0];
                DeleteKeyLight(third, third->keys[0]);
                first->child[1] = second->child[0];

                if (first->child[1] != NULL)
                {
                    first->child[1]->parent = first;
                }

                second->child[0] = second->child[1];
                second->child[1] = third->child[0];

                if (second->child[1] != NULL)
                {
                    second->child[1]->parent = second;
                }

                third->child[0] = third->child[1];
                third->child[1] = third->child[2];
                third->child[2] = NULL;
            }
        }
    }
    else if (parent->size == 1)
    {
        InsertKeyLight(leaf, parent->keys[0]);

        if (first == leaf && second->size == 2)
        {
            parent->keys[0] = second->keys[0];
            DeleteKeyLight(second, second->keys[0]);

            if (leaf->child[0] == NULL)
            {
                leaf->child[0] = leaf->child[1];
            }

            leaf->child[1] = second->child[0];
            second->child[0] = second->child[1];
            second->child[1] = second->child[2];
            second->child[2] = NULL;

            if (leaf->child[1] != NULL)
            {
                leaf->child[1]->parent = leaf;
            }
        }
        else if (second == leaf && first->size == 2)
        {
            parent->keys[0] = first->keys[1];
            DeleteKeyLight(first, first->keys[1]);

            if (leaf->child[1] == NULL)
            {
                leaf->child[1] = leaf->child[0];
            }

            leaf->child[0] = first->child[2];
            first->child[2] = NULL;

            if (leaf->child[0] != NULL)
            {
                leaf->child[0]->parent = leaf;
            }
        }
    }

    return parent;
}

//  easy cases with merging
Tree* CaseE(Tree* leaf)
{
    if (leaf == NULL)
    {
        return leaf;
    }

    Tree* parent = leaf->parent;

    if (parent->child[0] == leaf)
    {
        InsertKeyLight(parent->child[1], parent->keys[0]);
        parent->child[1]->child[2] = parent->child[1]->child[1];
        parent->child[1]->child[1] = parent->child[1]->child[0];

        if (leaf->child[0] != NULL)
        {
            parent->child[1]->child[0] = leaf->child[0];
        }
        else if (leaf->child[1] != NULL)
        {
            parent->child[1]->child[0] = leaf->child[1];
        }

        if (parent->child[1]->child[0] != NULL)
        {
            parent->child[1]->child[0]->parent = parent->child[1];
        }

        DeleteKeyLight(parent, parent->keys[0]);
        if (parent->child[0] != NULL)
        {
            free(parent->child[0]);
        }
        parent->child[0] = NULL;
    }
    else if (parent->child[1] == leaf)
    {
        InsertKeyLight(parent->child[0], parent->keys[0]);

        if (leaf->child[0] != NULL)
        {
            parent->child[0]->child[2] = leaf->child[0];
        }
        else if (leaf->child[1] != NULL)
        {
            parent->child[0]->child[2] = leaf->child[1];
        }

        if (parent->child[0]->child[2] != NULL)
        {
            parent->child[0]->child[2]->parent = parent->child[0];
        }

        DeleteKeyLight(parent, parent->keys[0]);
        if (parent->child[1] != NULL)
        {
            free(parent->child[1]);
        }
        parent->child[1] = NULL;
    }

    if (parent->parent == NULL)
    {
        Tree* tmp = NULL;

        if (parent->child[0] != NULL)
        {
            tmp = parent->child[0];
        }
        else
        {
            tmp = parent->child[1];
        }

        tmp->parent = NULL;
        if (parent != NULL)
        {
            free(parent);
        }
        return tmp;
    }

    return parent;
}

//  tree print (LNR)
void PrintTree(Tree* root)
{
    if (root == NULL)
    {
        return;
    }

    PrintTree(root->child[0]);

    for (int i = 0; i < root->size; ++i)
    {
        printf("%d ", root->keys[i]);
    }

    PrintTree(root->child[1]);
    PrintTree(root->child[2]);
}

int main()
{
    char sym;
    Tree* t = NULL;
    int key =0;
    while (scanf("%c", &sym) >= 1) {
        if (sym != 'p' && sym != 'q')
            scanf("%i", &key);
        switch (sym) {
            case 'a':
                t = Insert(t, key);
                break;
            case 'r':
                t = Delete(t, key);
                break;
            case 'f':
                if (Find(t, key)) {
                    puts("yes");
                }
                else {
                    puts("no");
                }
                break;
            case 'p':
                PrintTree(t);
                putchar('\n');
                break;
        }
    }

    return 0;
}