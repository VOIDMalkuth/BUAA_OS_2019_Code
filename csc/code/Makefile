modules			:= fibo main

.PHONY: all clean $(modules)

all: $(modules)

$(modules):
	gcc -I../include -c $@.c -o $@.o

clean:
	rm -rf *.o
