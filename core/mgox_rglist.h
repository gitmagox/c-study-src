/*双向环形链表*/
#ifndef MAGOX_RGLIST_A
#define MAGOX_RGLIST_A

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

typedef struct cListNode cListNode;
typedef struct cList cList;

//节点
struct cListNode
{
	cListNode *pre;
	cListNode *next;
	void *data;
};
//链表
struct cList
{
	cListNode *head;
	cListNode *tail;
	cList *iterator;
	uint32_t size;
};

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
void c_list_push_front( cList *list, void *data, int size );

/* 册除链表的尾结点 */
void _c_list_free_tail( cList *list );

/* 册除链表的头结点 */
void _c_list_free_head( cList *list );

/* 册除指点的结点 */
void _c_list_free_node( cList *list, cListNode *nd );

/* 取链表的头结点内容 */
void c_list_read_head( cList *list );

/* 取链表的尾结点内容 */
void c_list_read_tail( cList *list );

/* 取链表的头结点 */
cListNode *_c_list_read_head( cList *list );

/* 取链表的尾结点 */
cListNode *_c_list_read_tail( cList *list );

cListNode * c_list_node_create( int size )
{
    cListNode *nd = ( cListNode* )malloc( sizeof(cListNode) );

    assert( nd->data != NULL );
    if ( nd )
    {
        memset( nd, 0, sizeof( cListNode ) );
    }
    nd->data = malloc( size );
    return nd;
}

cList * c_list_init( int size )
{
    cList *list = ( cList* )malloc( sizeof( cList ) );

    cListNode *head = c_list_node_create( size ),
                *tail = c_list_node_create( size );
    int i = 0;

    assert( list != NULL );
    assert( head != NULL );
    assert( tail != NULL );

    memset( list, 0, sizeof( cList ) );

    list->head = head;
    list->tail = tail;
    //首尾相链
    list->head->next = list->tail;
    list->tail->pre = list->head;

    list->size = 0;

    return list;
}

void c_list_free( cList *list )
{
    uint32_t    i = 0;

    cListNode *node = NULL,*tmp;

    assert(list != NULL );

    node = list->head;

    while ( node )
    {
        free( node->data );
        tmp = node->next;
        free( node );
        node = tmp;
    }

    free( list );

}

void _c_list_push_back( cList *list, cListNode *nd )
{

    assert( nd != NULL );
    assert( list != NULL );

    list->tail->pre ->next = nd;
    nd->next = list->tail;
    nd->pre = list->tail->pre;
    list->tail->pre = nd;
    list->size++;
}

void _c_list_push_front(cList *list, cListNode *nd)
{

   assert( list != NULL );
   assert( nd != NULL );

   list->head->next ->pre = nd;
   nd->next = list->head->next;
   nd->pre = list->head;
}

void c_list_push_back( cList *list, void *value, int size )
{
    uint32_t pos = list->size;
    cListNode *nd = c_list_node_create( size );
    memcpy( nd->data, value, size );
    _c_list_push_back( list, nd );

}

void c_list_push_front( cList *list, void *value, int size )
{
    cListNode *nd = c_list_node_create( size );
    memcpy( nd->data, value, size );
    _c_list_push_front( list, nd );
}

void _c_list_free_back( cList *list )
{
    cListNode *nd = ( list->tail->pre );

    list->tail->pre = ( nd )->pre;
    ( nd )->pre->next = list->tail;
}
void c_list_free_front( cList *list )
{
    cListNode *nd = list->head->next;

    list->head->next = nd->next;
    nd->next->pre = list->head;

    free(nd);
    nd = NULL;
}

void c_list_free_node( cList *list, cListNode *nd )
{
    cListNode *now = list->head->next;

    assert( nd != list->head && nd != list->tail );

    while ( now )
    {
        if ( now->data == nd->data )
        {
            now->pre->next = now->next;
            now->next->pre = now->pre;
            free( now );
            break;
        }
        now = now->next;
    }

}

cListNode *_c_list_read_tail( cList *list )
{
    assert( list );
    return list->tail;
}

cListNode *_c_list_read_head( cList *list )
{
    assert( list );
    return list->head;
}

void *c_list_read_back( cList *list )
{
    return (_c_list_read_tail(list))->data;
}

void *c_list_read_front( cList *list )
{
    return (_c_list_read_head(list))->data;
}

#endif