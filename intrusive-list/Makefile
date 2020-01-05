CC := cc
CFLAGS := -g -O0 -Wall
VG := valgrind
VGFLAGS := --leak-check=full --show-leak-kinds=all --verbose

OBJS := list.o test.o
BIN := test


.PHONY: all test check clean clean_all
all: ${OBJS}
	${CC} ${CFLAGS} -o ${BIN} ${OBJS}

test: all
	${BIN}

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

check: all
	${VG} ${VGFLAGS} ${BIN}

clean:
	rm -rf ${OBJS}

clean_all: clean
	rm -rf ${BIN}

