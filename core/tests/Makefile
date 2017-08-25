check_magox_btree:check_magox_btree.c ../mgox_queue.h ../mgox_stack.h ../mgox_btree.h
	cc -g -o check_magox_btree check_magox_btree.c ../mgox_queue.h ../mgox_stack.h ../mgox_btree.h -lcheck
check_magox_queue:check_magox_queue.c ../mgox_queue.h
	cc -o check_magox_queue check_magox_queue.c ../mgox_queue.h -lcheck
.PHONY : clean
clean:
	rm check_magox_btree check_magox_queue
