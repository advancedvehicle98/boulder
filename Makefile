#lsdk;asdk
MAKEFLAGS += -rR

# ===============================================================

ifeq ($(VERBOSE),1)
	q = 
else
	q = @
endif

export VERBOSE q


# ===============================================================

this-makefile := $(lastword $(MAKEFILE_LIST))

export srctree  := $(abspath $(dir $(this-makefile)))
export objtree  := $(shell mkdir -p $(srctree)/out && cd $(srctree)/out && pwd)
export gentree  := $(shell mkdir -p $(srctree)/generated && cd $(srctree)/generated && pwd)
export modtree  := $(srctree)/modules
export scripts  := $(srctree)/scripts
export testtree := $(srctree)/tests

export VPATH   := $(srctree)

export kconfig := $(srctree)/Kconfig
export config  := $(srctree)/.config

export downloads := $(shell mkdir -p $(modtree)/downloads && cd $(modtree)/downloads && pwd)


# ===============================================================

include $(srctree)/config.mk
include $(shell touch $(config) && echo $(config))


# ===============================================================

phony += all
all: dsdl master slave


# ДАЛЬШЕ МОЖНО ПОМЕЩАТЬ СВОИ ЦЕЛИ

# ===============================================================

include $(scripts)/make/Makefile.dsdl
include $(scripts)/make/Makefile.master
include $(scripts)/make/Makefile.menuconfig
include $(scripts)/make/Makefile.rsync
include $(scripts)/make/Makefile.slave
include $(scripts)/make/Makefile.test

include $(scripts)/make/Makefile.build

include $(scripts)/make/Makefile.flash


# ===============================================================

export clean-dirs := $(objtree) $(gentree)

phony += clean
clean: $(clean-dirs) $(clean-targets)
	$(q)find ./ | grep '~' | xargs rm -f

phony += $(clean-dirs)
$(clean-dirs):
	$(q)rm -rf $@

phony += clean-downloads
clean-all-targets += clean-downloads
clean-downloads:
	$(q)rm -rf $(downloads)

phony += clean-all
clean-all: clean $(clean-all-targets)


# ===============================================================

.PHONY: $(phony)
