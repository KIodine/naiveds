CC := cc
AR := ar

CFLAGS := -Wall -Wextra
RELFLAGS := -O1
DBGFLAGS := -g -O0

ARFLAGS := -rcs

VG := valgrind
VGFLAGS := --show-leak-kinds=all --leak-check=full --verbose

PROJ_NAME := avl

BIN_NAME := $(PROJ_NAME)-test
LIB_BASENAME := lib$(PROJ_NAME)

INCDIR := include
SRCDIR := src
TSTDIR := test
OBJDIR := obj
LIBDIR := lib
BINDIR := bin

# Put all source files here, mind dependencies.
SRCFILES := avltree.c# avldbg.c
DBGFILES := avldbg.c
TSTFILES := main.c

SRCLIST := $(addprefix $(SRCDIR)/,$(SRCFILES))
OBJLIST := $(addprefix $(OBJDIR)/,$(patsubst %.c,%.o,$(SRCFILES)))

TSTLIST := $(addprefix $(TSTDIR)/,$(TSTFILES))
TSTOBJLIST := $(addprefix $(OBJDIR)/,$(patsubst %.c,%.o,$(TSTFILES)))

DBGLIST := $(addprefix $(SRCDIR)/,$(DBGFILES))
DBGOBJLIST := $(addprefix $(OBJDIR)/,$(patsubst %.c,%.o,$(DBGFILES)))

LIBSTATIC := $(LIB_BASENAME).a
LIBSHARED := $(LIB_BASENAME).so
LIBSTATICDST := $(LIBDIR)/$(LIBSTATIC)
LIBSHAREDDST := $(LIBDIR)/$(LIBSHARED)

BINFILE := $(BIN_NAME)
BINDST := $(BINDIR)/$(BINFILE)

# Modify compile flags.
ifeq ($(DEBUG), 1)
    CFLAGS += $(DBGFLAGS)
else
    CFLAGS += $(RELFLAGS)
endif
CFLAGS += -I./$(INCDIR)


.PHONY: all 

all: static shared buildtest

mkimmdir:
	mkdir -p ./$(LIBDIR) ./$(OBJDIR) ./$(BINDIR)

# makefile static patterns
$(OBJLIST): $(OBJDIR)/%.o: $(SRCDIR)/%.c mkimmdir
	$(CC) $(CFLAGS) -c $< -o $@

$(DBGOBJLIST): $(OBJDIR)/%.o: $(SRCDIR)/%.c mkimmdir
	$(CC) $(CFLAGS) -c $< -o $@

$(TSTOBJLIST): $(OBJDIR)/%.o: $(TSTDIR)/%.c mkimmdir
	$(CC) $(CFLAGS) -c $< -o $@

static: $(OBJLIST)
	$(AR) $(ARFLAGS) $(LIBSTATICDST) $<

shared: $(CFLAGS) += -fPIC
shared: $(OBJLIST)
	$(CC) $(CFLAGS) -shared -o $(LIBSHAREDDST) $<

buildtest: $(DBGOBJLIST) $(TSTOBJLIST) static
	$(CC) $(CFLAGS) -L./$(LIBDIR) -o $(BINDST) \
	$(DBGOBJLIST) $(TSTOBJLIST) -static -l$(PROJ_NAME)

runtest: buildtest
	./$(BINDST)
    
runcheck: buildtest
	$(VG) $(VGFLAGS) ./$(BINDST)

syntaxcheck:
	$(CC) -fsyntax-only $(CFLAGS) $(INCDIR)/* $(SRCDIR)/*

clean:
	rm -f $(OBJLIST)
	rm -f $(TSTOBJLIST)
	rm -f $(DBGOBJLIST)

clean_all: clean
	rm -f ./$(LIBSTATICDST) ./$(LIBSHAREDDST)
	rm -f ./$(BINDST)
