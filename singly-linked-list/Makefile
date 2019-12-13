CC 		:= cc
CFLAGS	:= -g -O0 -Wall
BIN 	:= test
OBJS 	:= sllist.o main.o

VG 		:= valgrind
VGFLAGS := --leak-check=full --show-leak-kinds=all --verbose

.PHONY: all test check clean clean_all

all: $(OBJS)
	${CC} -o ${BIN} ${CFLAGS} ${OBJS}

test: all
	./${BIN}

check: all
	${VG} ${VGFLAGS} ${BIN}

%.o: %.c
	${CC} -c $^ -o $@

clean:
	rm -f ${OBJS}

clean_all:
	rm -f ${OBJS} ${BIN}
