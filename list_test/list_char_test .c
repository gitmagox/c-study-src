#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "c_queue_1.h"

int main( int argc, char* argv[] )
{
	char elems[5] = {"a","b","c","d","e"};
	queue elemsStack;
	new_queue( &elemsStack, 5, sizeof(char));
	int i;	
	for ( i=0; i<5; i++ )
	{
		queue_push( &elemsStack, (elems+i) );
		printf( "%c\n",elems[i]);
	}
	
	int *name;
	
	for ( i=0; i<5; i++ )
	{
		queue_pop( &elemsStack, name );
		printf( "%c\n", *name );
	}
	return 0;
}
