/* 二叉树 */
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




