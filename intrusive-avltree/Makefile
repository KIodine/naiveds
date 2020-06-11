CC := cc
CFLAGS := -g -O0 -Wall -Wextra

VG := valgrind
VGFLAGS := --show-leak-kinds=all --leak-check=full --verbose

OBJS := main.o avltree.o avldbg.o
BIN := ./avltest

# TODO: more seperate build enviroment.

.PHONY: debug check clean clean_all

debug: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(BIN) 

debugrun: debug
	./$(BIN)

%.o: %.c
	$(CC) $< -c -o $@ $(CFLAGS)

check: debug
	$(VG) $(VGFLAGS) $(BIN) 

clean:
	rm -rf $(OBJS)

clean_all: clean
	rm -rf $(BIN)
