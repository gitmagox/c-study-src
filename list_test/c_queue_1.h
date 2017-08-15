/*队列*/
#ifndef C_QUEUE_H_1
#define C_QUEUE_H_1
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


typedef struct queue queue;

//队列
struct queue
{
	void *elems;//当前队列的地址
	void *head;//队头
	void *tail;//队尾
	int  size;  //单个元素大小
	int logLength;//当前队长度
	int allocLength;//允许的最大长度
};

//方法
/* 新建一个队列 */
void new_queue( queue *Q, int allocLength, int size );
/* 入队 */
void queue_push( queue *Q, void *node);
/* 出队 */
void queue_pop( queue *Q, void *node);
/* destory queue */
void queue_dispose( queue *Q );


//私有函数
static void queue_grow( queue *Q)
{
	Q->allocLength += 2;
	Q->tail = realloc( Q->tail, Q->allocLength*Q->size );
}
/* 新建一个队列 */
void new_queue( queue *Q, int allocLength, int size )
{
	assert( size > 0 );
	assert( allocLength > 0 );
	if( allocLength <=0 )
	{
		allocLength = 2;
	}
	Q->size = size;
	Q->logLength = 0;
	Q->allocLength = allocLength;
	Q->elems = malloc( allocLength*size );
	memset( Q->elems, 0, allocLength*size );
	Q->head = Q->elems;
	Q->tail = Q->elems;
}
/* 入队 */
void queue_push( queue *Q, void *node )
{
	if( Q->logLength == Q->allocLength )
	{
		queue_grow( Q );
	}
	void *target;
	if( Q->logLength == 0 )
	{
		target = (char *)Q->head;
	}
	else
	{
		target = (char *)Q->head +Q->size; 
	}
	memcpy( target, node, Q->size );
	Q->head  = target;
	Q->logLength ++;
	if( Q->logLength == 1 )
	{
		Q->tail = Q->head;
	}
}

/* 出队 */
void queue_pop( queue *Q, void *node )
{
	assert( Q->logLength > 0 );
	memcpy( node, Q->elems, Q->size );
	memset( Q->elems, '\0', Q->size );
	Q->tail = Q->elems = Q->elems+Q->size; 
	Q->logLength --;
}

/* 销毁队列 */
void queue_dispose( queue *Q )
{
	free( Q->elems );
}

#endif