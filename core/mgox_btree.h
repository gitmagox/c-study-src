/* 二叉树 */
#ifndef MAGOX_BTREE_A
#define MAGOX_BTREE_A
#include <assert.h>
#include "mgox_stack.h"
#include "mgox_queue.h"
typedef struct b_tree_node b_tree_node;
typedef int Type;
#define MAX_TREE_NODES 120;

struct b_tree_node {
	b_tree_node *parent;
	b_tree_node *left;
	b_tree_node *right;
	Type key
};

struct b_tree_root {
	b_tree_node *root;
	int logLength; //当前二叉树的大小
	int allocLength;//当前二叉树的允许的最大长度
}


//新建一个节点
b_tree_node new_b_tree_node( Type key );

//新建树,返回树的节点
b_tree_root new_b_tree( Type key );

//销毁一颗树
void destroy_b_tree( b_tree_root *root );

//添加一个节点
void add_b_tree_node( b_tree_root *root Type key );

//查找一个节点
b_tree_node search_b_tree( b_tree_root *root Type key );

//册除一个节点
void delete_b_tree_node( b_tree_root *root Type key );

//前序遍历
void pre_order_b_tree( b_tree_root *root );
//中序遍历
void in_order_b_tree( b_tree_root *root );
//后序遍历
void post_order_b_tree( b_tree_root *root );
//层序遍历
void level_order_b_tree( b_tree_root *root );

//函数实现
b_tree_node new_b_tree_node( Type key )
{
	b_tree_node *nd = ( b_tree_node* )malloc( sizeof(b_tree_node) );
	assert( nd != NULL );
    if ( nd )
    {
        memset( nd, 0, sizeof( b_tree_node ) );
    }
    nd.key = key;
    return nd;
}
//新建树,返回树的节点
b_tree_root new_b_tree( Type key )
{
	b_tree_root *root = ( b_tree_root* )malloc( sizeof(b_tree_root) );
	assert( root != NULL );
	b_tree_node *node = new_b_tree_node( key );
	assert( node != NULL );
	root->root = node;
	root->allocLength = MAX_TREE_NODES;
	root->logLength = 0;
	return root;
}
//添加一个节点,使用栈来处理
void add_b_tree_node( b_tree_root *root Type key )
{
	/* 新建一个栈来处理遍历过程 */
	stack S;
	StackNew( &S, MAX_TREE_NODES, sizeof( b_tree_node ) );
	StackPush( &S, root->root );
	b_tree_node * name;
	while( S->logLength > 0 )
	{	
		StackPop( &s, name );
		if( name.key == key )
		{
			name.key = key;
			root->logLength ++;
			StackDipose( &S );
			return 1;
		}
		else if( name.key < key )
		{
			if( name->left != NULL )
			{
				StackPush( &S, name->left );
			}
			else
			{
				b_tree_node *node = new_b_tree_node( key );
				name->left = node;
				root->logLength ++;
				StackDipose( &S );
				return 1; 
			}
		}
		else if( name.key > key )
		{
			if ( name->right != NULL )
			{
				StackPush( &S, name->right );
			}
			else
			{
				b_tree_node *node = new_b_tree_node( key );
				name->right = node;
				root->logLength ++;
				StackDipose( &S );
				return 1;
			}
		}
				
	}

}
//前序遍历
void pre_order_b_tree( b_tree_root *root )
{
	stack S;
	StackNew( &S, MAX_TREE_NODES, sizeof( b_tree_node ) );
	StackPush( &S, root->root );
	b_tree_node * name;
	while( S->logLength > 0 )
	{	
		StackPop( &s, name );
		printf("%d\n",name.key );
		if( name->left != NULL )
		{
			StackPush( &S, name->left );
		}	
		if ( name->right != NULL )
		{
			StackPush( &S, name->right );
		}
	}
	StackDipose( &S );			
}
//查找一个节点
b_tree_node search_b_tree( b_tree_root *root Type key )
{
	stack S;
	StackNew( &S, MAX_TREE_NODES, sizeof( b_tree_node ) );
	StackPush( &S, root->root );
	b_tree_node * name;
	while( S->logLength > 0 )
	{	
		StackPop( &s, name );
		if( name->key == key )
		{
			return name;
		}
		if( name->left != NULL )
		{
			StackPush( &S, name->left );
		}	
		if ( name->right != NULL )
		{
			StackPush( &S, name->right );
		}
	}
	StackDipose( &S );
	return 0;	
}
//中序遍历
void in_order_b_tree( b_tree_root *root )
{
	stack S;
	StackNew( &S, MAX_TREE_NODES, sizeof( b_tree_node ) );
	StackPush( &S, root->root );
	b_tree_node * name;
	while( S->logLength > 0 )
	{	
		if( name->left != NULL )
		{
			StackPush( &S, name->left );
		}	
		StackPop( &s, name );
		printf("%d\n",name.key );
		if ( name->right != NULL )
		{
			StackPush( &S, name->right );
		}
	}
	StackDipose( &S );
}
//后序遍历
void post_order_b_tree( b_tree_root *root )
{
	stack S;
	StackNew( &S, MAX_TREE_NODES, sizeof( b_tree_node ) );
	StackPush( &S, root->root );
	b_tree_node * name;
	while( S->logLength > 0 )
	{	
		if( name->left != NULL )
		{
			StackPush( &S, name->left );
		}	
		if ( name->right != NULL )
		{
			StackPush( &S, name->right );
		}
		StackPop( &s, name );
		printf("%d\n",name.key );
	}
	StackDipose( &S );
}
//销毁一颗树
void destroy_b_tree( b_tree_root *root )
{
	stack S;
	StackNew( &S, MAX_TREE_NODES, sizeof( b_tree_node ) );
	StackPush( &S, root->root );
	b_tree_node * name;
	while( S->logLength > 0 )
	{	
		if( name->left != NULL )
		{
			StackPush( &S, name->left );
		}	
		if ( name->right != NULL )
		{
			StackPush( &S, name->right );
		}
		StackPop( &s, name );
		free( name );
	}
	StackDipose( &S );
	free( root );
}
//层序遍历---广度优先
void level_order_b_tree( b_tree_root *root ){
	queue Q;
	new_queue( &Q, MAX_TREE_NODES, sizeof( b_tree_node ) );
	queue_push( &Q, root->root );
	b_tree_node *name;
	while( Q->logLength >0 )
	{
		if( name->left != NULL )
		{
			queue_push( &Q, name->left );
		}	
		if ( name->right != NULL )
		{
			queue_push( &Q, name->right );
		}
		queue_pop( &Q, name );
		printf("%d\n",name.key );
	}
}
//找二叉树最小节点
b_tree_node get_min_tree_node(b_tree_root *root)
{
	b_tree_node *name;
	name = root;
	while ( name->left != NULL )
	{
		name = name->left;
	}
	return name;
}

//找二叉树最大节点
b_tree_node get_max_tree_node(b_tree_root *root)
{
	b_tree_node *name;
	name = root;
	while ( name->right != NULL )
	{
		name = name->right;
	}
	return name;
}

//册二叉树最大节点
b_tree_node delete_max_tree_node(b_tree_root *root)
{
	b_tree_node *name;
	name = root;
	while ( name->right != NULL )
	{
		name = name->right;
	}
	name->parent->right = NULL;
	return name;
}
//册二叉树最小节点返回
b_tree_node delete_min_tree_node(b_tree_root *root)
{
	b_tree_node *name;
	name = root;
	while ( name->left != NULL )
	{
		name = name->left;
	}
	name->parent->left = NULL;
	return name;
}
//册二叉树的节点
void delete_b_tree_node( b_tree_root *root Type key )
{
	b_tree_node *node = search_b_tree( root, key );

	assert( node !=NULL );

	if( node->left == NULL && node -> right != NULL )
	{
		if( node->parent->left == node )
		{
			node->parent->left = node->right;
		}
		else 
		{
			node->parent->right = node->right;
		}
		
		node->right = NULL;
		free( node );
		root->logLength --;
	}
	if( node->left != NULL && node -> right == NULL )
	{
		if( node->parent->left == node )
		{
			node->parent->left = node->left;
		}
		else 
		{
			node->parent->right = node->left;
		}
		node->left = NULL;
		free( node );
		root->logLength --;
	}
	if( node->left == NULL && node -> right == NULL )
	{
		if( node->parent->left == node )
		{
			node->parent->left = NULL;
		}
		else 
		{
			node->parent->right = NULL;
		}
		free( node );
		root->logLength --;
	}
	if ( node->left != NULL && node -> right != NULL)
	{
		if( node->parent->left == node )
		{
			b_tree_node *new = delete_max_tree_node( node->right );
			new->left = node ->left;
			new->right = node->right;
			free( node );
			root->logLength --;
		}
		else 
		{
			b_tree_node *new = delete_max_tree_node( node->right );
			new->left = node->left;
			new->right = node->right;
			free( node );
			root->logLength --;
		}
	}
}


#endif