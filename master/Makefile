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

include $(scripts)/make/Makefile.canard


# ==============================================================

net-prefix = net
net-objs-y = \
	open_netlink_socket.o \
	try_set_linkinfo.o

can-dronecan-prefix = dronecan
can-dronecan-objs-y = handle_get_node_info.o

can-prefix = can
can-objs-y = data.o deinit.o init.o on_receive.o process_messages.o respond.o \
	set_bitrate.o should_accept_transfer.o \
	$(addprefix $(can-dronecan-prefix)/, $(can-dronecan-objs-y))

objs += \
	deinit.o init.o main.o parse_args.o run.o \
	$(addprefix $(can-prefix)/, $(can-objs-y)) \
	$(addprefix $(net-prefix)/, $(net-objs-y)) \
	$(addprefix dsdl/, $(dsdl-objs))

# --------------------------------------------

ifeq ($(CONFIG_MASTER_TOOLCHAIN),"")

ifneq ($(CONFIG_MASTER_NATIVE),y)
	$(error Неизвестный тулчейн. Вероятно, в .config ошибка)
endif

else # ifneq ($(CONFIG_MASTER_TOOLCHAIN),)

toolchain = $(CONFIG_MASTER_TOOLCHAIN)-

cc := $(toolchain)$(cc)
ld := $(toolchain)$(ld)

endif # ifneq ($(CONFIG_MASTER_TOOLCHAIN),)

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
