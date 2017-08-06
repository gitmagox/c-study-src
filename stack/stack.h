typedef struct 
{
	void * elems;    //当前栈的指针
	int elemSize;    //元素的长度
	int logLength;   //记录的个数
	int allocLength; //允许的长度
}stack;

void StackNew( stack *s, int elemSize  );
void StackDipose( stack *s );
void StackPush( stack *s, void *elemAddr );
void StackPop( stack *s ,void *elemAddr );