all: game robot-00.so

game: main.c common.h graphics.h robot.h
	cc main.c -o game -lSDL2 -ldl -lm

robot-00.so: robot-00.c robot-00.h
	cc -shared robot-00.c -o robot-00.so 

.PHONY: clean

clean:
	rm game robot-00.so
