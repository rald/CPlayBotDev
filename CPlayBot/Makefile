all: game robot-00.so

game: main.c common.h graphics.h robot.h
	gcc main.c -o game -lSDL2 -ldl -lm

robot-00.so: robot-00.c robot-00.h
	gcc -shared robot-00.c -o robot-00.so 

.PHONY: style run clean

style:
	astyle -t2 -p -xg --style=java *.c *.h

run:
	./game robot-00 robot-00 robot-00 robot-00 robot-00 robot-00 robot-00 robot-00 robot-00 robot-00

clean:
	rm game robot-00.so

