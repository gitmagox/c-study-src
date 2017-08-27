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
