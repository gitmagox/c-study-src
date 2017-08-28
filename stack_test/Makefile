all: stack_poit_test
stack_poit_test:stack_poit_test.c ../core/mgox_queue.h ../core/mgox_stack.h ../core/mgox_btree.h
	cc -g -o stack_poit_test stack_poit_test.c ../core/mgox_queue.h ../core/mgox_stack.h  ../core/mgox_btree.h
.PHONY : clean
clean:
	rm stack_poit_test
