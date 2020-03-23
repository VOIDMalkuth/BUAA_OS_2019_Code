INCLUDES := -I../include
CC        := gcc


%.o: %.c
	    $(CC) $(INCLUDES) -c $<

.PHONY: all clean

all: test_queue


test_queue: test_queue.o
	    $(CC) test_queue.o -o test_queue


clean:
		rm -rf *.o
		rm -rf test_queue
