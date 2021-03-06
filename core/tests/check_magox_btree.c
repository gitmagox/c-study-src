/*
 *-----------------------------
 *    btree c-check unit test
 *    Author: Magox
 *    Update time : 2017-8-24
 *----------------------------
 */

#include <stdlib.h>
#include "check.h"
#include "../mgox_btree.h"
//test new node
START_TEST(test_new_b_tree_node)
{
    b_tree_node *node;
    node = new_b_tree_node( 5 );
    ck_assert_int_eq(node->key, 5);
    free( node );
}
END_TEST
//test new tree
START_TEST(test_new_b_tree)
{
    b_tree_root *tree;
    tree = new_b_tree( 5 );
    ck_assert_int_eq(tree->root->key, 5);
    ck_assert_int_eq(tree->logLength, 1);
    ck_assert_int_eq(tree->allocLength, 120 );
    free( tree->root );
    free( tree );
}
END_TEST
//test destroy
START_TEST(test_destroy_b_tree)
{
    b_tree_root *tree;
    tree = new_b_tree( 5 );
    tree = new_b_tree( 5 );
    ck_assert_int_eq(tree->root->key, 5);
    destroy_b_tree( tree );
}
END_TEST
//test add node
START_TEST(test_add_b_tree_node)
{
    b_tree_root *tree;
    tree = new_b_tree( 5 );
    ck_assert_int_eq(tree->root->key, 5);
    add_b_tree_node( tree, 8 );
    ck_assert_int_eq(tree->root->right->key, 8);
    add_b_tree_node( tree, 7);
    ck_assert_int_eq(tree->root->right->left->key, 7);
    add_b_tree_node( tree, 4);
    ck_assert_int_eq(tree->root->left->key, 4);
    add_b_tree_node( tree, 3);
    ck_assert_int_eq(tree->root->left->left->key, 3);
    destroy_b_tree( tree );
}
END_TEST
//test search
START_TEST(test_search_b_tree)
{
    b_tree_root *tree;
    tree = new_b_tree( 5 );
    add_b_tree_node( tree, 6);
    add_b_tree_node( tree, 4);
    b_tree_node *search = search_b_tree( tree , 4 );
    ck_assert_int_eq( search->key, 4 );
    destroy_b_tree( tree );
}
END_TEST
//test delete node
START_TEST(test_delete_b_tree_node)
{
    b_tree_root *tree;
    tree = new_b_tree( 5 );
    add_b_tree_node( tree, 6);
    add_b_tree_node( tree, 4);
    delete_b_tree_node( tree, 4 );
    ck_assert_int_eq(tree->root->key, 5);
    ck_assert_int_eq(tree->root->right->key,6);
    delete_b_tree_node( tree, 6 );
    ck_assert_int_eq(tree->root->key, 5);
    delete_b_tree_node( tree, 5);
    destroy_b_tree( tree );
}
END_TEST
//test pre order
START_TEST(test_pre_order_b_tree)
{
    b_tree_root *tree;
    tree = new_b_tree( 5 );
    add_b_tree_node( tree, 8);
    add_b_tree_node( tree, 7);
    add_b_tree_node( tree, 4);
    add_b_tree_node( tree, 3);
    printf( "pre_order_list\n" );
    pre_order_b_tree( tree );
    destroy_b_tree( tree );
}
END_TEST
//test pre order_a
START_TEST(test_pre_order_b_tree_a)
{
    b_tree_root *tree;
    tree = new_b_tree( 5 );
    add_b_tree_node( tree, 8);
    add_b_tree_node( tree, 7);
    add_b_tree_node( tree, 4);
    add_b_tree_node( tree, 3);
    printf( "pre_order_list_a\n" );
    pre_order_b_tree_a( tree );
    destroy_b_tree( tree );
}
END_TEST

//in order
START_TEST(test_in_order_b_tree)
{
    b_tree_root *tree;
    tree = new_b_tree( 5 );
    add_b_tree_node( tree, 8);
    add_b_tree_node( tree, 7);
    add_b_tree_node( tree, 4);
    add_b_tree_node( tree, 3);
    printf( "in_order_list\n" );
    in_order_b_tree( tree );
    destroy_b_tree( tree );
}
END_TEST
//post order
START_TEST(test_post_order_b_tree)
{
    b_tree_root *tree;
    tree = new_b_tree( 5 );
    add_b_tree_node( tree, 8);
    add_b_tree_node( tree, 7);
    add_b_tree_node( tree, 4);
    add_b_tree_node( tree, 3);
    printf( "post_order_list\n" );
    post_order_b_tree( tree );
    destroy_b_tree( tree );
}
END_TEST
//level
START_TEST(test_level_order_b_tree)
{
    b_tree_root *tree;
    tree = new_b_tree( 5 );
    add_b_tree_node( tree, 8);
    add_b_tree_node( tree, 7);
    add_b_tree_node( tree, 4);
    add_b_tree_node( tree, 3);
    printf( "level_order_list\n" );
    level_order_b_tree( tree );
    destroy_b_tree( tree );
}
END_TEST

/*    test:
    b_tree_node* get_min_tree_node(b_tree_root *root)
    b_tree_node* get_max_tree_node(b_tree_root *root)
*/

START_TEST(test_get_max_min_tree_node)
{
    b_tree_root *tree;
    b_tree_node *node;
    tree = new_b_tree( 5 );
    add_b_tree_node( tree, 8);
    add_b_tree_node( tree, 7);
    add_b_tree_node( tree, 4);
    add_b_tree_node( tree, 3);
    node = get_min_tree_node( tree);
    ck_assert_int_eq(node->key, 3);
    node = get_max_tree_node( tree );
    ck_assert_int_eq(node->key, 8);
    printf( "after test get max nin node list\n" );
    pre_order_b_tree( tree );
    destroy_b_tree( tree );
}
END_TEST

/*    test:
    b_tree_node* delete_min_tree_node(b_tree_root *root)
    b_tree_node* delete_max_tree_node(b_tree_root *root)
*/

START_TEST(test_delete_max_min_tree_node)
{
    b_tree_root *tree;
    b_tree_node *node;
    tree = new_b_tree( 5 );
    add_b_tree_node( tree, 8);
    add_b_tree_node( tree, 7);
    add_b_tree_node( tree, 4);
    add_b_tree_node( tree, 3);
    node = delete_min_tree_node( tree );
    ck_assert_int_eq(node->key, 3);
    node = delete_max_tree_node( tree );
    ck_assert_int_eq(node->key, 8);
    printf( "after test delete max nin node list\n" );
    pre_order_b_tree( tree );
    //level_order_b_tree( tree );
    destroy_b_tree( tree );
}
END_TEST

Suite * magox_btree_suite(void)
{
    Suite *s;
    TCase *tc_core;
    s = suite_create("magox_btree");
    tc_core = tcase_create("Core");
    //unit test
    tcase_add_test( tc_core, test_new_b_tree_node );
    tcase_add_test( tc_core, test_new_b_tree );
    tcase_add_test( tc_core, test_add_b_tree_node );
    tcase_add_test( tc_core, test_destroy_b_tree );
    tcase_add_test( tc_core, test_search_b_tree );
    tcase_add_test( tc_core, test_delete_b_tree_node );
    tcase_add_test( tc_core, test_pre_order_b_tree );
    tcase_add_test( tc_core, test_pre_order_b_tree_a );
    tcase_add_test( tc_core, test_in_order_b_tree );
    tcase_add_test( tc_core, test_post_order_b_tree );
    tcase_add_test( tc_core, test_get_max_min_tree_node );
    tcase_add_test( tc_core, test_delete_max_min_tree_node );
    tcase_add_test( tc_core, test_level_order_b_tree );
    suite_add_tcase(s, tc_core);
    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;
    s = magox_btree_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
