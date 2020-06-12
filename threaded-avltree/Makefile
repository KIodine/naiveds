CC := cc
AR := ar

CFLAGS := -Wall -Wextra -std=c99
RELFLAGS := -O1
DBGFLAGS := -g -O0

ARFLAGS := -rcs

VG := valgrind
VGFLAGS := --show-leak-kinds=all --leak-check=full --verbose

PROJ_NAME := threaded-avl

BIN_NAME := $(PROJ_NAME)-test
LIB_BASENAME := lib$(PROJ_NAME)

INCDIR := include
SRCDIR := src
TSTDIR := test
OBJDIR := obj
LIBDIR := lib
BINDIR := bin

# Put all source files here, mind dependencies.
SRCFILES := avltree.c list.c
DBGFILES := avldbg.c
TSTFILES := main.c

# --- imm targets to build ---
SRCLIST := $(addprefix $(SRCDIR)/,$(SRCFILES))
OBJLIST := $(addprefix $(OBJDIR)/,$(patsubst %.c,%.o,$(SRCFILES)))

TSTLIST := $(addprefix $(TSTDIR)/,$(TSTFILES))
TSTOBJLIST := $(addprefix $(OBJDIR)/,$(patsubst %.c,%.o,$(TSTFILES)))

DBGLIST := $(addprefix $(SRCDIR)/,$(DBGFILES))
DBGOBJLIST := $(addprefix $(OBJDIR)/,$(patsubst %.c,%.o,$(DBGFILES)))
# --- end of imm targets ---

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


.PHONY: all static shared

all: static shared buildtest


# make directory before all starts.
$(OBJLIST) $(TSTOBJLIST) $(DBGOBJLIST): |$(OBJDIR)
$(OBJDIR):
	mkdir -p ./$(LIBDIR) ./$(OBJDIR) ./$(BINDIR)

# Setup build flag for each set of object.
# makefile static-patterns.
# target-specific variable also apply to static-patterns.
$(OBJLIST): $(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DBGOBJLIST): $(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TSTOBJLIST): $(OBJDIR)/%.o: $(TSTDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# NOTE: If you want to be able to build static and shared library seperately,
#	using the file itself as target, so the other targets depend on library
#	wouldn't build library as the target has been invoked.
static: $(LIBSTATICDST)
$(LIBSTATICDST): $(OBJLIST)
	$(AR) $(ARFLAGS) $(LIBSTATICDST) $<

shared: $(LIBSHAREDDST)
# Using `target-specific variables`.
$(LIBSHAREDDST): CFLAGS += -fPIC
$(LIBSHAREDDST): $(OBJLIST)
	$(CC) $(CFLAGS) -shared -o $(LIBSHAREDDST) $^

# or `-Wl,rpath <dir>` can make the produced ELF using shared library?
# NOTE: This will make the ELF search for the libdir of project as a static
# target, the executable will not work anywhere besides the place it built.
# NOTE: If a prerequisite is a target, it will be execute anyway?

$(BINDST): $(DBGOBJLIST) $(TSTOBJLIST) $(LIBSHAREDDST)
	$(CC) $(CFLAGS) -L./$(LIBDIR) -o $@ \
	$(DBGOBJLIST) $(TSTOBJLIST) -Wl,-rpath,$(LIBDIR) -l$(PROJ_NAME)
buildtest: $(BINDST)

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

# TODO: Put object header deps.
#	Get deps of a src using `cc -MM -I./include src/*
#	Add obj lists.
# NOTE: How do make treat these rules that already had more explicit
#	target?
#	-> See 4.11 Multiple Rules For One Target
#	   In short, make merges additional rules into a target.

# --- Complete dependencies. ---
obj/avldbg.o: src/avldbg.c include/avldbg.h include/avltree.h include/list.h \
 include/list.h
obj/avltree.o: src/avltree.c include/avltree.h include/list.h \
 include/avlinc.h
obj/list.o: src/list.c include/list.h
