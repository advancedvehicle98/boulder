BUILD_DIR ?= ./build

SRC_DIR = ./src
INC_DIR = ./include

# -------------------------------------------------------------------------

COMPONENTS = can net

CAN_TARGETS = data.o deinit.o init.o set_bitrate.o

NET_TARGETS = \
	open_netlink_socket.o \
	try_set_linkinfo.o

OBJS = deinit.o init.o main.o parse_args.o run.o \
	$(foreach T,$(CAN_TARGETS),can/$(T)) \
	$(foreach T,$(NET_TARGETS),net/$(T))

# -------------------------------------------------------------------------

TARGETS = $(foreach O,$(OBJS),$(BUILD_DIR)/$(O))

# --------------------------------------------------------------

TOOLCHAIN ?= $(CONFIG_MASTER_TOOLCHAIN)

# --------------------------------------------------------------

CC = $(TOOLCHAIN)-gcc

# строго плюс равно, т.к. в корневом Makefile также могут быть
# добавлены флаги

# мы используем gnu99, потому что иначе gcc не видит
# структуры из net/if.h (см. например src/can/init.c)
CFLAGS += -O2 -std=gnu99

CFLAGS += -I $(INC_DIR)

# --------------------------------------------------------------

LD = $(CC)

LIBS = c m

LDFLAGS += $(foreach L,$(LIBS),-l$(L))

# --------------------------------------------------------------

PHONY = all
all: build_dir compile link

# --------------------------------------------------------------

PHONY += compile
compile: $(TARGETS)

$(BUILD_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

# --------------------------------------------------------------

PHONY += link
link:
	$(LD) $(LDFLAGS) $(TARGETS) -o $(BUILD_DIR)/out/master

# --------------------------------------------------------------

PHONY += build_dir
build_dir: $(foreach C,$(COMPONENTS),$(BUILD_DIR)/$(C))
	mkdir -p $(BUILD_DIR)/out

$(BUILD_DIR)/%:
	mkdir -p $@

# --------------------------------------------------------------

PHONY += clean
clean:
	rm -rf $(BUILD_DIR)

# --------------------------------------------------------------

.PHONY: $(PHONY)
