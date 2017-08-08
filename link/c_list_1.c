#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "c_list_1.h"

cListNode * c_list_node_create( int size )
{
    cListNode *nd = ( cListNode* )malloc( sizeof(cListNode) );

    assert( nd->data!=NULL );
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

    assert(list != NULL );
    assert(head != NULL );
    assert(tail != NULL );

    memset(list,0,sizeof(cList));

    list->head = head;
    list->tail = tail;

    list->head->next = list->tail;
    list->tail ->pre = list->head;

    list->size = 0;

    return list;
}
void c_list_free(cList *list)
{
    uint32_t    i = 0;

    cListNode *node = NULL,*tmp;

    assert(list != NULL );

    node = list->head;

    while ( node )
    {
        free(node->data);
        tmp = node->next;
        free(node);
        node = tmp;
    }

    free(list);

}


