CC := cc
CFLAG := -g -O0 -Wall -Wextra

VG := valgrind
VFLAG := --leak-check=full --show-leak-kinds=all --verbose

OBJS := hashmap.o main.o
BIN := out


.PHONY: all run check clean clean_all

all: $(OBJS)
	$(CC) $(CFLAG) $(OBJS) -o $(BIN)

run: all
	./$(BIN)

check: all
	$(VG) $(VFLAG) ./$(BIN)

%.o: %.c
	$(CC) $(CFLAG) -c $< -o $@

clean:
	rm -f $(OBJS)

clean_all: clean
	rm -f $(BIN)
