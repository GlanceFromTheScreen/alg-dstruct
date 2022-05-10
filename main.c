#include <stdio.h>
#include <stdlib.h>

typedef struct Treap
{
    int key;  //  x
    int priority;  //  y
    struct Treap* left;
    struct Treap* right;
} Treap;

typedef struct SplitOut
{
    struct Treap* right;
    struct Treap* left;
} SplitOut;

Treap* CreateTreap(int key)
{
    Treap* newNode = (Treap*)malloc(sizeof(Treap));

    if (newNode == NULL)
    {
        return NULL;
    }

    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->priority = rand() % 100;

    return newNode;
}

SplitOut Split(Treap* tree, int k)  //  devide into 2 trees: with less keys and more and equal ones
{
    if (tree == NULL)
    {
        SplitOut pair;
        pair.right = NULL;
        pair.left = NULL;
        return pair;
    }

    if (k > tree->key)
    {
        SplitOut pair = Split(tree->right, k);
        tree->right = pair.left;
        pair.left = tree;
        return pair;
    }
    else
    {
        SplitOut pair = Split(tree->left, k);
        tree->left = pair.right;
        pair.right = tree;
        return pair;
    }
}

Treap* Merge(Treap* tree1, Treap* tree2)  //  keys in the left subtree must be less, than keys in the right one
{
    if (tree2 == NULL)
    {
        return tree1;
    }

    if (tree1 == NULL)
    {
        return tree2;
    }

    else if (tree1->priority > tree2->priority)
    {
        tree1->right = Merge(tree1->right, tree2);
        return tree1;
    }
    else
    {
        tree2->left = Merge(tree1, tree2->left);
        return tree2;
    }
}

Treap* Add(Treap* tree, int key)
{
    SplitOut pair = Split(tree, key);
    Treap* newTreap = CreateTreap(key);

    if (newTreap == NULL)
    {
        return NULL;
    }

    return Merge(Merge(pair.left, newTreap), pair.right);
}

Treap* Remove(Treap* tree, int key)
{
    if (tree == NULL)
    {
        return NULL;
    }

    SplitOut pair1 = Split(tree, key);
    SplitOut pair2 = Split(pair1.right, key + 1);
    tree = Merge(pair1.left, pair2.right);

    free(pair2.left);

    return tree;
}

Treap* FindElement(Treap* tree, int key)
{
    Treap* result = NULL;

    if (tree == NULL)
    {
        return NULL;
    }

    if (tree->key == key)
    {
        return tree;
    }

    result = FindElement(tree->left, key);

    if (result != NULL)
    {
        return result;
    }

    result = FindElement(tree->right, key);

    if (result != NULL)
    {
        return result;
    }
    else
    {
        return NULL;
    }
}

void DestroyTree(Treap* tree)
{
    if (tree != NULL)
    {
        DestroyTree(tree->left);
        DestroyTree(tree->right);
        free(tree);
    }

    return;
}


int main()
{
    Treap* T = CreateTreap(11);
    Treap* V = CreateTreap(10);
    //V->right = T;
    //printf("%d %d\n", T->key, V->key);
    //Merge(V, T);
    V = Add(V, 9);
    V = Add(V, 4);
    V = Add(V, 20);
    V = Add(V, 6);
    V = Add(V, 17);
    V = Remove(V, 17);
    //printf("%p\n", FindElement(V, 17));
    SplitOut TV = Split(V, 11);
    //printf("%d %d\n", TV.right->key, TV.left->key);


    char sym;
    int key;
    Treap* tree = NULL;

    while (fscanf(stdin, "%c %d", &sym, &key) > 0)
    {
        Treap* check = NULL;

        if (sym == 'a')
        {
            check = FindElement(tree, key);

            if (check == NULL)
            {
                tree = Add(tree, key);
            }
        }

        if (sym == 'r')
        {
            if(FindElement(tree, key) != NULL)
                tree = Remove(tree, key);
        }

        if (sym == 'f')
        {
            check = FindElement(tree, key);

            if (check == NULL)
            {
                fprintf(stdout, "no\n");
            }
            else
            {
                fprintf(stdout, "yes\n");
            }
        }
    }


    return 0;
}
