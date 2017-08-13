/* 二叉树 */
#include <assert.h>

typedef struct b_tree_node b_tree_node;
typedef int Type;

struct b_tree_node {
	b_tree_node *parent;
	b_tree_node *left;
	b_tree_node *right;
	Type key
};

struct b_tree_root {
	b_tree_node *root;
}


//新建一个节点
b_tree_node _new_tree_node( Type key );

//新建树,返回树的节点
b_tree_root new_b_tree( Type key );

//销毁一颗树
void destroy_b_tree( b_tree_root *root );

//添加一个节点
void add_b_tree_node( b_tree_root *root Type key );

//册除一个节点
void delete_b_tree_node( b_tree_root *root Type key );

//前序遍历
void pre_order_b_tree( b_tree_node *node );
//中序遍历
void in_order_b_tree( b_tree_node *node );
//后序遍历
void post_order_b_tree( b_tree_node *node );

//函数实现
b_tree_node _new_tree_node( Type key )
{
	b_tree_node *nd = ( b_tree_node* )malloc( sizeof(b_tree_node) );
	assert( nd != NULL );
    if ( nd )
    {
        memset( nd, 0, sizeof( b_tree_node ) );
    }
    nd->key = key;
    return nd;
}
//新建树,返回树的节点
b_tree_root new_b_tree( Type key )
{
	b_tree_root *root = ( b_tree_root* )malloc( sizeof(b_tree_root) );
	assert( root != NULL );
	b_tree_node *node = _new_tree_node( key );
	assert( node != NULL );
	root->root = node;
	return root;
}