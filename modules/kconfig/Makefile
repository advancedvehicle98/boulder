prefix        := $(menuconfig-prefix)
local-srctree := $(abspath .)

target := $(objtree)/$(prefix)/mconf



# ===============================================================

phony += all
all: objtree-prefixes parser-generated $(target)


# ===============================================================

cflags  := -I. -std=gnu99 -Wfatal-errors -D_GNU_SOURCE
ldflags := 
libs    := ncursesw

# ----------------------------------------------------------------

lxdialog_objs := checklist.o inputbox.o menubox.o textbox.o util.o yesno.o

objs := mconf.o zconf.tab.o \
	$(addprefix lxdialog/, $(lxdialog_objs))


# ----------------------------------------------------------------

include $(scripts)/make/Makefile.build


# ===============================================================

phony += parser-generated
parser-generated: force zconf.y zconf.l
	$(q)$(yacc) -o zconf.tab.c --defines=zconf.tab.h -t -l zconf.y
	$(q)$(lex) -o zconf.lex.c -L zconf.l


# ===============================================================

phony += clean
clean:
	$(q)rm -f zconf.tab.c zconf.tab.h  zconf.lex.c


# ===============================================================

.PHONY: $(phony)
