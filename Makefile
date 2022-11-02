CFLAGS = -g -std=c99 -Wall -Wextra -pedantic -Wno-misleading-indentation -Wno-shift-negative-value -O2 `pkg-config --cflags glfw3 glew`
LIBS = `pkg-config --libs glfw3 glew` -lm
SRC = src/main.c src/gui.c src/tinyfiledialogs.c

fpsR: ${SRC}
	${CC} ${CFLAGS} ${SRC} -o fpsR ${LIBS}
