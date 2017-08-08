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

