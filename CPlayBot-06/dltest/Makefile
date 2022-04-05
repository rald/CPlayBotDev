all: test robot-00.so

test: test.c
	cc test.c -o test -ldl

robot-00.so: robot-00.c
	cc -shared robot-00.c -o robot-00.so

.PHONY: clean

clean:
	rm test robot-00.so
