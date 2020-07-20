CC := cc
CFLAGS := -g -Wall -O0 -pedantic
VG := valgrind
VGFLAGS := --show-leak-kinds=all --leak-check=full --verbose

OBJS := main.o binheap.o
BIN := binheap-test


.PHONY: buildtest runtest runcheck clean clean_all

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

$(BIN): ${OBJS}
	${CC} ${CFLAGS} -o ${BIN} ${OBJS}
buildtest: $(BIN)

runtest: buildtest
	./${BIN}

runcheck: buildtest
	${VG} ${VGFLAGS} ${BIN}

clean:
	rm -rf ${OBJS}

clean_all: clean
	rm -rf ${BIN}

# --- supply deps ---
# Invoke `gcc -MM {<src>,...}` to generate target dependencies.

binheap.o: binheap.c binheap.h
main.o: main.c binheap.h
