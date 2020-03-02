CFLAGS			:= --std=c99

CC				:= gcc

targets			:= palindrome

.PHONY: all clean

all: $(targets)

$(targets):
	$(CC) $(CFLAGS) $@.c -o $@

clean:
	for t in $(targets);			\
		do							\
			rm -rf $$t;				\
		done;
