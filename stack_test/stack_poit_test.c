#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "stack.h"

typedef struct b_tree_node b_tree_node;
typedef struct b_tree_root b_tree_root;
typedef int Type;
#define MAX_TREE_NODES 120

struct b_tree_node {
	b_tree_node *parent;
	b_tree_node *left;
	b_tree_node *right;
	Type key;
};

struct b_tree_root {
	b_tree_node *root;
	int logLength;
	int allocLength;
};


//函数实现
b_tree_node* new_b_tree_node( Type key )
{
	b_tree_node *nd = ( b_tree_node* )malloc( sizeof(b_tree_node) );
	assert( nd != NULL );
	if ( nd )
	{
		memset( nd, 0, sizeof( b_tree_node ) );
	}
	nd->key = key;
	nd->parent = NULL;
	nd->left = NULL;
	nd->right = NULL;
	return nd;
}
//新建树,返回树的节点
b_tree_root* new_b_tree( Type key )
{
	b_tree_root *root = ( b_tree_root* )malloc( sizeof(b_tree_root) );
	assert( root != NULL );
	b_tree_node *node = new_b_tree_node( key );
	assert( node != NULL );
	root->root = node;
	root->allocLength = MAX_TREE_NODES;
	root->logLength = 1;
	return root;
}
//添加一个节点,使用栈来处理
void add_b_tree_node( b_tree_root *root, Type key )
{
	/* 新建一个栈来处理遍历过程 */
	stack S;
	b_tree_node *name;
	StackNew( &S, 4, sizeof( b_tree_node* ) );
	StackPush( &S, &(root->root) );
	while( StackCount(&S) > 0 )
	{
		StackPop( &S, &name );
		if( name->key == key )
		{
			name->key = key;
			root->logLength ++;
			StackDipose( &S );
			return 1;
		}
		else if( name->key > key )
		{
			if( name->left != NULL )
			{
				StackPush( &S, &(name->left) );
			}
			else
			{
				b_tree_node *node = new_b_tree_node( key );
				node->parent = name;
				name->left = node;
				root->logLength ++;
				StackDipose( &S );
				return 1;
			}
		}
		else if( name->key < key )
		{
			if ( name->right != NULL )
			{
				StackPush( &S, &(name->right) );
			}
			else
			{
				b_tree_node *node = new_b_tree_node( key );
				node->parent = name;
				name->right = node;
				root->logLength ++;
				StackDipose( &S );
				return 1;
			}
		}
	}
}

//后序遍历
void post_order_b_tree( b_tree_root *root )
{
	stack S;
	StackNew( &S, MAX_TREE_NODES, sizeof( b_tree_node* ) );
	StackPush( &S, &(root->root) );
	b_tree_node * name;
	b_tree_node * namel;
	name = root->root;
	while( StackCount(&S) > 0 )
	{
		if( name->left != NULL )
		{
			StackPush( &S, &(name->left) );
		}
        StackPop( &S, &namel );
        printf("%d\n",namel->key );
		if ( name->right != NULL )
		{
			StackPush( &S, &(name->right) );
		}
        name = namel;
	}
	StackDipose( &S );
}

//销毁一颗树
void destroy_b_tree( b_tree_root *root )
{
	stack S;
	StackNew( &S, MAX_TREE_NODES, sizeof( b_tree_node* ) );
	StackPush( &S, &(root->root) );
	b_tree_node * name;
	name = root->root;
	while( StackCount(&S) > 0 )
	{
	    if( name->left != NULL )
        {
            StackPush( &S, &(name->left) );
        }
        if( name->right != NULL )
        {
            StackPush( &S, &(name->right) );
        }
        StackPop( &S, &name );
        printf("%d\n",name->key);
        free( name );
	}
	StackDipose( &S );
	free( root );
}

int main( int argc, char* argv[] )
{
    b_tree_root *root;
    root = new_b_tree( 5 );
    //printf("%d\n",root->root->key);
    add_b_tree_node( root, 8 );
    add_b_tree_node( root, 7 );
    add_b_tree_node( root, 4 );
    post_order_b_tree( root );
    destroy_b_tree( root );
	return 0;
}
