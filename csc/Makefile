objects			:= code/*.o
modules			:= objfiles

.PHONY: all fibo clean objfiles

all: fibo

objfiles:
	make --directory=code

fibo: $(modules)
	gcc $(objects) -o fibo

clean:
	make --directory=code clean


