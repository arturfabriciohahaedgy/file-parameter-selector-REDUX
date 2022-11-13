CFLAGS = -std=c99 -Wall -Wextra -pedantic -Wno-misleading-indentation -Wno-shift-negative-value -pipe -O2 `pkg-config --cflags glfw3 glew`
LIBS = `pkg-config --libs glfw3 glew` -lm
SRC = src/main.c src/gui.c src/tinyfiledialogs.c src/files.c
OBJ = ${SRC:.c=.o}

all: fpsR

fpsR: ${OBJ}
	${CC} ${CFLAGS} ${OBJ} -o fpsR ${LIBS}

.c.o:
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -f fpsR src/*.o
