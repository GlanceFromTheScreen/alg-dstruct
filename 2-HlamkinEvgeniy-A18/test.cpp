#include <gtest/gtest.h>
#include "sort.h"
#include <string.h>

TEST(CREATE_TEST, create)
{
    char x[] = "1";
    list* list1 = create(x);
    EXPECT_EQ(*x, *list1->value);
    EXPECT_EQ(list1->next, nullptr);
    remove_list(list1);
}

TEST(ADD_ELEMENT_END_TEST, add_element_end)
{
    char x[] = "21";
    char y[] = "2";
    list* list1 = create(x);
    add_element_end(y, list1);
    EXPECT_EQ(0, strcmp(list1->next->value, y));
    remove_list(list1);

}

TEST(CHANGE_TWO_NODES_TEST, change_two_nodes)
{
    char x[] = "1";
    char y[] = "2";
    list* list1 = create(x);
    add_element_end(y, list1);
    add_element_end(x, list1);
    change_two_nodes(1,list1);
    EXPECT_EQ(*y, *list1->next->next->value);
    remove_list(list1);
}

TEST(COMPARE_TEST, compare)
{
    char x[] = "123 0 9\0";
    char y[] = "123 5\0";
    char z[] = "123 1\0";
    char t[] = "123 1\0";

    list* list1 = create(x);
    list* list2 = create(y);
    list* list3 = create(z);
    list* list4 = create(t);

    EXPECT_EQ(compare(list1->value, list2->value), -1);
    EXPECT_EQ(compare(list2->value, list3->value), 1);
    EXPECT_EQ(compare(list3->value, list4->value), 0);

    remove_list(list1);
    remove_list(list2);
    remove_list(list3);
    remove_list(list4);
}

TEST(INSERT_TEST, insert)
{
    char x[] = "1\0";
    char y[] = "2\0";
    char z[] = "3\0";
    char t[] = "4\0";

    list* list1 = create(x);
    add_element_end(y, list1);
    add_element_end(z, list1);
    add_element_end(t, list1);

    insert(1, 3, list1);

    EXPECT_EQ(strcmp(list1->next->value, t), 0);
    remove_list(list1);
}

TEST(ADD_NODE_TEST, add_node)
{
    char x[] = "1\0";
    char y[] = "2\0";
    char z[] = "3\0";

    list* list1 = create(x);
    add_element_end(y, list1);
    list* list2 = add_node(z, 0, list1);

    EXPECT_EQ(strcmp(list2->value, z), 0);

    remove_list(list2);
}

TEST(SORT_TEST, sort)
{
    char x[] = "4\0";
    char y[] = "3\0";
    char z[] = "2\0";



    list* list1 = create(x);
    add_element_end(y, list1);
    add_element_end(z, list1);


    list* list2 = sort(list1);

    EXPECT_EQ(strcmp(list2->value, z), 0);
    EXPECT_EQ(strcmp(list2->next->value, y), 0);
    EXPECT_EQ(strcmp(list2->next->next->value, x), 0);
    EXPECT_EQ(list2->next->next->next, nullptr);
}



