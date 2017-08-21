#include <stdlib.h>
#include "check.h"
#include "../mgox_btree.h"

START_TEST(test_new_b_tree_node)
{
    b_tree_node *node;

    node = new_b_tree_node( 5 );
    ck_assert_int_eq(node->key, 5);
    free( node );
}
END_TEST

START_TEST(test_new_b_tree)
{
    b_tree_root *tree;

    tree = new_b_tree( 5 );
    ck_assert_int_eq(tree->root->key, 5);
    ck_assert_int_eq(tree->logLength, 0);
    ck_assert_int_eq(tree->allocLength, 120 );
    free( tree->root );
    free( tree );
}
END_TEST
//检测内存是否已经干净
START_TEST(test_destroy_b_tree)
{
    b_tree_root *tree;
    tree = new_b_tree( 5 );
    b_tree_root *new = new_b_tree( 5 );
    memset( tree,'\0',sizeof( b_tree_root ) );
    destroy_b_tree( tree );
    ck_assert_mem_eq( tree, new , sizeof( b_tree_root ));

}

END_TEST

Suite * magox_btree_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("magox_btree");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test( tc_core, test_new_b_tree_node );
    tcase_add_test( tc_core, test_new_b_tree );
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
