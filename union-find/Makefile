CC = cc

CFLAGS = -Wall -Wextra
CF_DBG = -g -O0 -fsanitize=address
CF_REL = -O2 -DNDEBUG $(CF_REL_NOWARN)
CF_REL_NOWARN = -Wno-unused-function

# TODO: conditional build.
ifeq ($(RELEASE), 1)
	CFLAGS += $(CF_REL)
else
	CFLAGS += ${CF_DBG}
endif

VG = valgrind

VFLAGS = --show-leak-kinds=all --leak-check=full --verbose


PROJ_NAME := union-find
INCDIR := ./src
SRCDIR := ./src
TSTDIR := ./test
OBJDIR := ./objs
BINDIR := ./bin

CFLAGS += -I$(INCDIR)

TSTBIN = ${BINDIR}/${PROJ_NAME}-test

TSTOBJS = main.o
OBJS    = unionfind.o

OBJDST    = $(addprefix $(OBJDIR)/,$(OBJS))
TSTOBJDST = $(addprefix $(OBJDIR)/,$(TSTOBJS))

# --- prepare ---
$(OBJDST) $(TSTOBJDST): |$(OBJDIR)
$(OBJDIR):
	mkdir -p $(OBJDIR) $(BINDIR)

# --- objs ---
$(OBJDST): $(OBJDIR)/%.o: $(SRCDIR)/%.c
	${CC} $(CFLAGS) -o $@ -c $<

$(TSTOBJDST): $(OBJDIR)/%.o: $(TSTDIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(TSTBIN): $(OBJDST) $(TSTOBJDST)
	$(CC) $(CFLAGS) -o $@ $^

buildtest: $(TSTBIN)
runtest: buildtest
	$(TSTBIN)

runcheck: buildtest
	$(VG) $(VFLAGS) $(TSTBIN)

# --- cleanups ---
clean:
	rm -f ./objs/*.o

clean_all: clean
	rm -f $(TSTBIN)

# --- auto generated by `gcc -MM <src>` ---
_gen_prereq:
	${CC} -MM -I${INCDIR} ${SRCDIR}/*.c ${TSTDIR}/*.c

# --- update using `_gen_prereq` ---
unionfind.o: src/unionfind.c src/unionfind.h
main.o: test/main.c src/unionfind.h
