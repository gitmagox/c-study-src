#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "../core/mgox_btree.h"


int main( int argc, char* argv[] )
{
    b_tree_root *root;
    root = new_b_tree( 5 );
    //printf("%d\n",root->root->key);
    add_b_tree_node( root, 8 );
    add_b_tree_node( root, 7 );
    add_b_tree_node( root, 9 );
    add_b_tree_node( root, 4 );
    add_b_tree_node( root, 3 );
    level_order_b_tree( root );
    printf( "delete node 8\n" );
    delete_b_tree_node( root, 8 );
    level_order_b_tree( root );
//    printf( "delete node 3\n" );
//    delete_b_tree_node( root, 7 );
//    level_order_b_tree( root );
    //post_order_b_tree( root );
    destroy_b_tree( root );
	return 0;
}
