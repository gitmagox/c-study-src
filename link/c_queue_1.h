/*队列*/
#define C_QUEUE_H_1

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
	s->allocLength += 1;
	s->elems = realloc( s->tail, s->allocLength*s->size );
}
/* 新建一个队列 */
void new_queue( queue *Q, int allocLength, int size )
{
	assert( size > 0 );
	assert( allocLength > 0 );
	if( allocLength <=0 )
	{
		allocLength = 1;
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
	if( s->logLength == s->allocLength )
	{
		queue_grow( Q );
	}
	void * target = (char *)Q->elems +Q->logLength*Q->size;
	memcpy( target, node, Q->size );
	Q->head = target;
	Q->logLength ++;
}

/* 出队 */
void queue_pop( queue *Q, void *node )
{
	assert( s->logLength > 0 );
	memcpy( node, Q->head, Q->size );
	memset( Q->elems, '\0', size );
	Q->elems = Q->head = Q->elems + Q->size; 
	Q->logLength --;
}

/* 销毁队列 */
void queue_dispose( queue *Q )
{
	free( Q->queue );
}