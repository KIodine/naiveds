CC := cc
AR := ar

# Flags when building shared object.
LDFLAGS = -Wl,--version-script=$(CFGDIR)/version.lds

CFLAGS := -Wall -Wextra -std=c99
REL_NOWARN := -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter
RELFLAGS := -O2 -DNDEBUG $(REL_NOWARN)
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
CFGDIR := cfg

# Put all source files here, mind dependencies.
LIBOBJS := avltree.o list.o
DBGOBJS := avldbg.o
TSTOBJS := main.o

# --- imm targets to build ---
LIBOBJDST := $(addprefix $(OBJDIR)/,$(LIBOBJS))
DBGOBJDST := $(addprefix $(OBJDIR)/,$(DBGOBJS))
TSTOBJDST := $(addprefix $(OBJDIR)/,$(TSTOBJS))
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
$(LIBOBJDST) $(TSTOBJDST) $(DBGOBJDST): |$(OBJDIR)
$(OBJDIR):
	mkdir -p ./$(LIBDIR) ./$(OBJDIR) ./$(BINDIR)

# --- start build obj groups ---
# Setup build flag for each set of object.
# makefile static-patterns.
# target-specific variable also apply to static-patterns.
$(LIBOBJDST): $(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DBGOBJDST): $(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TSTOBJDST): $(OBJDIR)/%.o: $(TSTDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
# --- end build obj groups ---

# NOTE: If you want to be able to build static and shared library seperately,
#	using the file itself as target, so the other targets depend on library
#	wouldn't build library as the target has been invoked.
static: $(LIBSTATICDST)
$(LIBSTATICDST): $(LIBOBJDST)
	$(AR) $(ARFLAGS) $(LIBSTATICDST) $^

shared: $(LIBSHAREDDST)
# Using `target-specific variables`.
$(LIBSHAREDDST): CFLAGS += -fPIC
$(LIBSHAREDDST): $(LIBOBJDST)
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -o $(LIBSHAREDDST) $^

# or `-Wl,rpath <dir>` can make the produced ELF using shared library?
# NOTE: This will make the ELF search for the libdir of project as a static
# target, the executable will not work anywhere besides the place it built.
# NOTE: If a prerequisite is a target, it will be execute anyway?

$(BINDST): $(DBGOBJDST) $(TSTOBJDST) $(LIBSHAREDDST)
	$(CC) $(CFLAGS) -L./$(LIBDIR) -o $@ \
	$(DBGOBJDST) $(TSTOBJDST) -Wl,-rpath,$(LIBDIR) -l$(PROJ_NAME)
buildtest: $(BINDST)

runtest: buildtest
	./$(BINDST)
    
runcheck: buildtest
	$(VG) $(VGFLAGS) ./$(BINDST)

syntaxcheck:
	$(CC) -fsyntax-only $(CFLAGS) $(INCDIR)/* $(SRCDIR)/*

clean:
	rm -f $(LIBOBJDST)
	rm -f $(TSTOBJDST)
	rm -f $(DBGOBJDST)

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
