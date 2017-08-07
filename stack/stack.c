#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "stack.h"

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
	assert( s->elems != NULL );
}
//desroy stack
void StackDipose( stack *s )
{
	free( s->elems );
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
//*elemAddr:弹出一元素。
void StackPop( stack *s, void *elemAddr )
{	
	assert( s->logLength  > 0 );
	void *source = (char *)s->elems + ( s->logLength-1 )*s->elemSize;
	memcpy( elemAddr, source, s->elemSize );
	s->logLength--;
}

