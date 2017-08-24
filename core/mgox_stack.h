/*
 *-----------------------------
 *    Stack <栈>
 *    Author: Magox
 *    Update time : 2017-8-24
 *----------------------------
 */
#ifndef MAGOX_STACK_A
#define MAGOX_STACK_A
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
typedef struct 
{
	void * elems;
	int elemSize;
	int logLength;
	int allocLength;
}stack;
void StackNew( stack *s, int allocLength , int elemSize  );
void StackDipose( stack *s );
void StackPush( stack *s, void *elemAddr );
void StackPop( stack *s ,void *elemAddr );
int StackCount( stack *s );

//私有函数
static void StackGrow( stack *s)
{
	s->allocLength *= 2;
	s->elems = realloc( s->elems, s->allocLength*s->elemSize );
}
//new stack
void StackNew( stack *s, int allocLength , int elemSize )
{
	assert( elemSize > 0 );
	assert( allocLength > 0  );
	if ( allocLength <= 0 )	
	{
		allocLength = 4;		
	}
	s->elemSize = elemSize;
	s->logLength = 0;
	s->allocLength = allocLength;
	s->elems = malloc( allocLength*elemSize );
	memset( s->elems, 0, allocLength*elemSize);
	assert( s->elems != NULL );
}
//desroy stack
void StackDipose( stack *s )
{
	int i;
	for( i=0; i < s->logLength ; i ++)
	{
		free( s->elems+i );
	}
}
//push elements
void StackPush( stack *s, void *elemAddr )
{
	if ( s->logLength == s->allocLength )
	{
		StackGrow( s );
	}
	void * target = (char *)s->elems + s->logLength * s->elemSize;
	memcpy( target, elemAddr, s->elemSize );
	s->logLength ++;
}
//pop element
void StackPop( stack *s, void *elemAddr )
{	
	assert( s->logLength  > 0 );
	void *source = (char *)s->elems + ( s->logLength-1 )*s->elemSize;
	memcpy( elemAddr, source, s->elemSize );
	s->logLength--;
}
//求当前元素
int StackCount( stack *s )
{
    return s->logLength;
}
#endif