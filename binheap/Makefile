CC := cc
CFLAGS := -g -Wall -O0 -pedantic
VG := valgrind
VGFLAGS := --show-leak-kinds=all --leak-check=full --verbose

OBJS := main.o binheap.o
BIN := ./test


.PHONY: all test check clean clean_all
all: ${OBJS}
	${CC} ${CFLAGS} -o ${BIN} ${OBJS}

test: all
	${BIN}

check: all
	${VG} ${VGFLAGS} ${BIN}

%.o: %.c
	${CC} ${CFLAGS} -c $^ -o $@

clean:
	rm -rf ${OBJS}

clean_all: clean
	rm -rf ${BIN}
