CC 		:= cc
CFLAGS 	:= -Wall -O0 -g
OBJS 	:= main.o bstree.o
BIN 	:= test

VALGRIND 	:= valgrind
VFLAGS 		:= --leak-check=full --show-leak-kinds=all --verbose


.PHONY: all run check clean
all: ${OBJS}
	${CC} ${CFLAGS} -o ${BIN} ${OBJS}

run: all
	./${BIN}

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@ 

test: all
	valgrind ${VFLAGS} ./${BIN}

clean:
	rm -f ${OBJS}

clean_all:
	rm -f ${OBJS} ${BIN}
