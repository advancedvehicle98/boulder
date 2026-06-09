include $(config)


# ==============================================================

local         := $(srctree)/$(master-prefix)
local-srctree := $(local)/src
prefix        := $(master-prefix)

target := $(objtree)/$(prefix)/master


# ==============================================================

phony += all
all: objtree-prefixes $(target)


# ==============================================================

net-prefix := net
net-objs-y := \
	open_netlink_socket.o \
	try_set_linkinfo.o

can-prefix := can
can-objs-y := data.o deinit.o init.o set_bitrate.o

objs := \
	deinit.o init.o main.o parse_args.o run.o \
	$(addprefix $(can-prefix)/, $(can-objs-y)) \
	$(addprefix $(net-prefix)/, $(net-objs-y))

# --------------------------------------------

ifeq ($(CONFIG_MASTER_TOOLCHAIN),)
ifneq ($(CONFIG_MASTER_NATIVE),y)
	$(error Неизвестный тулчейн. Вероятно, в .config ошибка)
endif
endif

toolchain = $(CONFIG_MASTER_TOOLCHAIN)-

cc := $(toolchain)$(cc)
ld := $(toolchain)$(ld)

# -------------------------------------------------------------

inc-paths := \
	$(local)/include \
	$(dsdl-path)/include

cflags += \
	$(addprefix -I,$(inc-paths)) \
	-std=gnu99

libs += c m

# -------------------------------------------------------------

include $(scripts)/make/Makefile.build


# ==============================================================

.PHONY: $(phony)
