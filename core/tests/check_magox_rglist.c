/*
 *-----------------------------
 *    rglist c-check unit test
 *    Author: Magox
 *    Update time : 2017-9-3
 *----------------------------
 */

#include <stdlib.h>
#include "check.h"
#include "../mgox_rglist.h"
//test new node
START_TEST(test_c_list_node_create)
{
    cListNode *node;
    node = c_list_node_create( int );
    ck_assert_int_eq(sizeof(node->data), sizeof(int));
    free( node );
}
END_TEST


Suite * magox_btree_suite(void)
{
    Suite *s;
    TCase *tc_core;
    s = suite_create("magox_rglist");
    tc_core = tcase_create("Core");
    //unit test
    tcase_add_test( tc_core, test_c_list_node_create );
 
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
