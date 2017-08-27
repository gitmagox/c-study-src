/*
 *----------------------------------
 *    stack<栈> c-check unit test
 *    Author: Magox
 *    Update time : 2017-8-27
 *---------------------------------
 */

#include <stdlib.h>
#include "check.h"
#include "../mgox_stack.h"

//test new queue
START_TEST(test_StackNew)
{
    stack S;
    StackNew( &S, 4, sizeof( Type ) );
}
END_TEST
//test StackPush_pop
START_TEST(test_StackPush_pop)
{
    char elems[5] = {'b','a','e','g','o'};
    queue q;
    StackNew( &q, 5, sizeof(char));
    int i;
    for ( i=0; i<5; i++ )
    {
    	StackPush( &q, (elems+i) );
    }
    char name;
    for ( i=0; i<5; i++ )
    {
    	StackPop( &q, &name );
    	ck_assert_int_eq( name, *(elems+i) );
    }
}
END_TEST
//test StackDipose
START_TEST(test_StackDipose)
{
    char elems[5] = {'b','a','e','g','o'};
    queue q;
    StackNew( &q, 5, sizeof(char));
    int i;
    for ( i=0; i<5; i++ )
    {
    	StackPush( &q, (elems+i) );
    }
    StackDipose( &q );
}
END_TEST
//test StackCount
START_TEST(test_StackCount)
{
    char elems[5] = {'b','a','e','g','o'};
    queue s;
    StackNew( &s, 5, sizeof(char));
    int i;
    for ( i=0; i<5; i++ )
    {
       StackPush( &q, (elems+i) );
       ck_assert_int_eq( StackCount( &q ), (i+1) );
    }
    char name;
    for ( i=0; i<5; i++ )
    {
        StackPop( &q, &name );
        ck_assert_int_eq( StackCount( &q ), (4-i) );
        ck_assert_int_eq( name, *(elems+4-i) );
    }
}
END_TEST
//magox stack suite
Suite * magox_stack_suite(void)
{
    Suite *s;
    TCase *tc_core;
    s = suite_create("magox_btree");
    tc_core = tcase_create("Core");
    //unit test
    tcase_add_test( tc_core, test_StackNew );
    tcase_add_test( tc_core, test_StackPush_pop );
    tcase_add_test( tc_core, test_StackCount );
    tcase_add_test( tc_core, test_StackDipose );
    suite_add_tcase(s, tc_core);
    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;
    s = magox_stack_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
