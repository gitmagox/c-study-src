/*双向链表*/
#ifndef C_LIST_H_1
#define C_LIST_H_1

//节点
typedef struct cListNode
{
	cListNode *pre;
	cListNode *next;
	void *data;
};
//链表
typedef struct cList
{
	cList *head;
	cList *tail;
	cList *iterator;
	uint32_t size;
}

//方法

/* 新建一个节点 */
cListNode*  c_list_node_create( int size );

/* 初始化一个双向链表 */
cList* c_list_init( int size );

/* 册除一个链表 */
void c_list_free( cList *list );

/* 在当前节点后添加一个节点 */
void _c_list_push_back( cList *list, cListNode *nd );

/* 在当前节点前添加一个节点 */
void _c_list_push_front( cList *list, cListNode *nd );

/* 在当前节点后添加一个节点的内容 */
void c_list_push_back( cList *list, void *data, int size );
/* 在当前节点前添加一个节点的内容 */
void _c_list_push_front( cList *list, void *data, int size );