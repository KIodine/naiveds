CC := cc
DBGFLAGS := -g -O0 -Wall -Wextra
RLSFLAGS := -O2 -Wall -Wextra

VG := valgrind
VGFLAGS := --leak-check=full --show-leak-kinds=all --verbose

OBJS := avltree.o main.o
BIN := ./test


.PHONY: debug check clean clean_all

debug: $(OBJS)
	$(CC) $(OBJS) $(DBGFLAGS) -o $(BIN) 

debugrun: debug
	./$(BIN)

%.o: %.c
	$(CC) $< -c -o $@ $(DBGFLAGS)

check: debug
	$(VG) $(VGFLAGS) $(BIN) 

clean:
	rm -rf $(OBJS)

clean_all: clean
	rm -rf $(BIN)
