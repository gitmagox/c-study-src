#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "c_queue_1.h"

int main( int argc, char* argv[] )
{
	int elems[5] = {123,456,789,345,-235};
	queue elemsStack;
	new_queue( &elemsStack, 5, sizeof(int));
	int i;	
	for ( i=0; i<5; i++ )
	{
		queue_push( &elemsStack, (elems+i) );
		printf( "%d\n",elems[i]);
	}
	
	int *name;
	
	for ( i=0; i<5; i++ )
	{
		queue_pop( &elemsStack, name );
		printf( "%d\n", *name );
	}
	return 0;
}
