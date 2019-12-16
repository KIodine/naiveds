CC 		:= cc
CFLAGS 	:= -g -Wall -O0 -pedantic -std=c99
VG 		:= valgrind
VFLAGS 	:= --leak-check=full --show-leak-kinds=all --verbose

OBJS 	:= dllist.o main.o
BIN 	:= test


.PHONY: all test check clean clean_all
all: ${OBJS}
	${CC} ${CFLAGS} -o ${BIN} ${OBJS}

test: all
	./${BIN}

%.o: %.c
	${CC} ${CFLAGS} -c $^ -o $@

check: all
	${VG} ${VFLAGS} ./${BIN}

clean:
	rm -f ${OBJS}

clean_all:
	rm -f ${OBJS} ${BIN}
