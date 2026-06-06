BUILD_DIR ?= ./build

SRC_DIR = ./src
INC_DIR = ./include

COMPONENTS = chips drivers platforms

PLATFORM_OBJS = init.o

CHIP_OBJS = general_purpose_handler.o init.o reset.o startup.o

OBJS = handle_exit.o init.o main.o run.o \
	$(foreach COBJ,$(CHIP_OBJS),chips/$(CONFIG_SLAVE_CHIP)/$(COBJ)) \
	$(foreach POBJ,$(PLATFORM_OBJS),platforms/$(CONFIG_SLAVE_PLATFORM)/$(POBJ))

TARGETS = $(foreach O,$(OBJS),$(BUILD_DIR)/$(O))

ALL_DIRS = \
	$(SRC_DIR) \
	$(INC_DIR)/slave \
	$(INC_DIR)/slave/chips \
	$(INC_DIR)/slave/chips/arm \
	$(foreach C,$(COMPONENTS),$(SRC_DIR)/$(C)) \
	$(foreach P,$(SLAVE_PLATFORM_LIST),$(SRC_DIR)/platforms/$(P)) \
	$(foreach C,$(SLAVE_CHIP_LIST),$(SRC_DIR)/chips/$(C))

# -------------------------------------------------------------------------

TOOLCHAIN ?= $(CONFIG_SLAVE_TOOLCHAIN)

# -------------------------------------------------------------------------

CC = $(TOOLCHAIN)-gcc

CFLAGS += \
	$(CONFIG_SLAVE_CFLAGS) \
	-nostdlib -O2 -std=gnu99 \
	-ffunction-sections \
	-fdata-sections \
	-ffreestanding

CFLAGS += -I $(INC_DIR)

# -------------------------------------------------------------------------

LD = $(CC)

LIBS =

LDFLAGS += $(CONFIG_SLAVE_LDFLAGS) \
	$(foreach L,$(LIBS),-l$(L)) \
	-nostdlib -nostartfiles \
	-T src/chips/$(CONFIG_SLAVE_CHIP)/link.ld

# -------------------------------------------------------------------------

PHONY = all
all: $(BUILD_DIR) compile link chip_specific

# chip_specific определяется в mk-конфигах под каждый камень
# в скриптах используются пути, относительные к ./slave (мы сейчас
# здесь и находимся)

# chip_specific должен давать на выходе $(BUILD_DIR)/out/slave
# на основе $(BUILD_DIR)/out/slave.elf из цели link (см. ниже)

# -------------------------------------------------------------------------

PHONY += compile
compile: $(TARGETS)

$(BUILD_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(BUILD_DIR)/%.o: src/%.S
	$(CC) $(CFLAGS) -a $^ -o $@

# -------------------------------------------------------------------------

PHONY += link
link:
	$(LD) $(LDFLAGS) $(TARGETS) -o $(BUILD_DIR)/out/slave.elf


BUILD_DIR_CONTENTS = \
	$(COMPONENTS) \
	chips/$(CONFIG_SLAVE_CHIP) \
	platforms/$(CONFIG_SLAVE_PLATFORM)

$(BUILD_DIR): $(foreach D,$(BUILD_DIR_CONTENTS),$(BUILD_DIR)/$(D))
	mkdir -p $@/out

$(BUILD_DIR)/%:
	mkdir -p $@

# -------------------------------------------------------------------------

PHONY += clean
clean: remove_squiggles
	rm -rf $(BUILD_DIR)

PHONY += remove_squiggles
remove_squiggles:
	rm -rf $(foreach D,$(ALL_DIRS),$(D)/*~)

# -------------------------------------------------------------------------

include ../config/slave/chips/$(CONFIG_SLAVE_CHIP)/targets.mk

PHONY += chip_specific

# -------------------------------------------------------------------------

.PHONY: $(PHONY)
