#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "stack.h"

int main( int argc, char* argv[] )
{
	char elems[5] = {'b','a','e','g','o'};
	stack elemsStack;
	StackNew( &elemsStack, 5, sizeof(char));
	int i;	
	for ( i=0; i<5; i++ )
	{
		StackPush( &elemsStack, (elems+i) );
		printf( "%c\n",elems[i]);
	}
	
	char *name;
	
	for ( i=0; i<5; i++ )
	{
		StackPop( &elemsStack, name );
		printf( "%s\n", name );
	}
	return 0;
}
