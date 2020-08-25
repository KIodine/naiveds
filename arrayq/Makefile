CC = cc
CFLAGS = -Wall -O0 -g -std=c99

OBJS = fifoq.o main.o

BIN = test-fifoq

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS)
buildtest: $(BIN)

runtest: $(BIN)
	./$(BIN)

clean:
	rm -f *.o $(BIN)

# --- set deps ---
fifoq.o: fifoq.c fifoq.h