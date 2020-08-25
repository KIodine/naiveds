CC := cc
VG := valgrind

CFLAGS := -Wall -Wextra -std=c99
DBGFLAGS = -g -O0 -fsanitize=address
RELFLAGS = -O2 -DNDEBUG ${RELNOWARN}
RELNOWARN = 

VGFLAGS := --show-leak-kinds=all --leak-check=full --verbose

OBJS := main.o binheap.o
BIN := binheap-test

# TODO: Rewrite.
ifeq (${DEBUG}, 1)
	CFLAGS += ${DBGFLAGS}
else
	CFLAGS += ${RELFLAGS}
endif

.PHONY: buildtest runtest runcheck clean clean_all

${OBJS}: %.o: %.c
	${CC} ${CFLAGS} -o $@ -c $<

$(BIN): ${OBJS}
	${CC} ${CFLAGS} -o ${BIN} ${OBJS}
buildtest: $(BIN)

runtest: buildtest
	./${BIN}

runcheck: buildtest
	${VG} ${VGFLAGS} ./${BIN}

clean:
	rm -rf ${OBJS}

clean_all: clean
	rm -rf ${BIN}

# --- supply deps ---
# Invoke `gcc -MM {<src>,...}` to generate target dependencies.

binheap.o: binheap.c binheap.h
main.o: main.c binheap.h
