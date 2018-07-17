all: check_magox_btree check_magox_queue check_magox_stack check_magox_rglist.c
check_magox_btree:check_magox_btree.c ../mgox_queue.h ../mgox_stack.h ../mgox_btree.h
	cc -g -o check_magox_btree check_magox_btree.c ../mgox_queue.h ../mgox_stack.h ../mgox_btree.h -lcheck -lpthread `pkg-config --cflags --libs check` -lrt
check_magox_queue:check_magox_queue.c ../mgox_queue.h
	cc -o check_magox_queue check_magox_queue.c ../mgox_queue.h -lpthread `pkg-config --cflags --libs check` -lcheck -lrt
check_magox_stack:check_magox_stack.c ../mgox_stack.h
	cc -o check_magox_stack check_magox_stack.c ../mgox_stack.h -lpthread `pkg-config --cflags --libs check` -lcheck -lrt
check_magox_rglist:check_magox_rglist.c ../mgox_rglist.h
	cc -o check_magox_rglist check_magox_rglist.c ../mgox_rglist.h -lpthread `pkg-config --cflags --libs check` -lcheck -lrt
.PHONY : clean
clean:
	rm check_magox_btree check_magox_queue check_magox_stack
