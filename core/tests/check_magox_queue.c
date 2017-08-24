/*
 *----------------------------------
 *    queue<队列> c-check unit test
 *    Author: Magox
 *    Update time : 2017-8-24
 *---------------------------------
 */

#include <stdlib.h>
#include "check.h"
#include "../mgox_queue.h"

//test new queue
START_TEST(test_new_queue)
{
    queue Q;
    tree = new_queue( &Q, 4, sizeof( Type ) );
}
END_TEST
//test queue_push_pop
START_TEST(test_queue_push_pop)
{
    char elems[5] = {"a","b","c","d","e"};
    queue q;
    new_queue( &q, 5, sizeof(char));
    int i;
    for ( i=0; i<5; i++ )
    {
    	queue_push( &q, (elems+i) );
    }
    char name;

    for ( i=0; i<5; i++ )
    {
    	queue_pop( &q, &name );
    	ck_assert_char_eq( name, *elems[i])
    }
    return 0;
}
END_TEST

END_TEST
//test queue_dispose
START_TEST(test_queue_dispose)
{
    char elems[5] = {"a","b","c","d","e"};
    queue q;
    new_queue( &q, 5, sizeof(char));
    int i;
    for ( i=0; i<5; i++ )
    {
    	queue_push( &q, (elems+i) );
    }
    char name;
    queue_dispose( &q );
}
END_TEST
//test queue_count
START_TEST(test_queue_count)
{
    char elems[5] = {"a","b","c","d","e"};
    queue q;
    new_queue( &q, 5, sizeof(char));
    int i;
    for ( i=0; i<5; i++ )
    {
       queue_push( &q, (elems+i) );
       ck_assert_int_eq( queue_count( &q ), (i+1) );
    }
    char name;
    for ( i=0; i<5; i++ )
    {
        queue_pop( &q, &name );
        ck_assert_int_eq( queue_count( &q ), (5-i+1) );
        ck_assert_char_eq( name, *elems[i])
    }
}
END_TEST

Suite * magox_queue_suite(void)
{
    Suite *s;
    TCase *tc_core;
    s = suite_create("magox_btree");
    tc_core = tcase_create("Core");
    //unit test
    tcase_add_test( tc_core, test_new_queue );
    tcase_add_test( tc_core, test_queue_push_pop );
    tcase_add_test( tc_core, test_queue_count );
    tcase_add_test( tc_core, test_queue_dispose );
    suite_add_tcase(s, tc_core);
    return s;
}

int main(void)
{
	int number_failed;
    Suite *s;
    SRunner *sr;
    s = magox_queue_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
