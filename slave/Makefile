BUILD_DIR ?= ./build

SRC_DIR = ./src
INC_DIR = ./include

# -------------------------------------------------------------------------

PLATFORM_TARGETS = init.o

# -------------------------------------------------------------------------

CHIP_CAN_TARGETS = init.o

CHIP_COMPONENTS = can

CHIP_TARGETS = general_purpose_handler.o init.o reset.o startup.o \
	$(foreach C,$(CHIP_CAN_TARGETS),can/$(C))

# -------------------------------------------------------------------------

CAN_TARGETS = init.o

COMPONENTS = can chips drivers platforms

OBJS = handle_exit.o init.o main.o run.o \
	$(foreach COBJ,$(CHIP_TARGETS),chips/$(CONFIG_SLAVE_CHIP)/$(COBJ)) \
	$(foreach POBJ,$(PLATFORM_TARGETS),platforms/$(CONFIG_SLAVE_PLATFORM)/$(POBJ)) \
	$(foreach C,$(CAN_TARGETS),can/$(C))

# -------------------------------------------------------------------------

TARGETS = $(foreach O,$(OBJS),$(BUILD_DIR)/$(O))

# -------------------------------------------------------------------------

TOOLCHAIN ?= $(CONFIG_SLAVE_TOOLCHAIN)

# -------------------------------------------------------------------------

CC = $(TOOLCHAIN)-gcc

CFLAGS += \
	$(CONFIG_SLAVE_CFLAGS) \
	-nostdlib -O2 -std=gnu99 \
	-ffunction-sections \
	-fdata-sections \
	-ffreestanding \
	-DCONFIG_SLAVE_CHIP_$(CONFIG_SLAVE_CHIP) \
	-DCONFIG_SLAVE_PLATFORM_$(CONFIG_SLAVE_PLATFORM_NAME) \
	$(foreach D,$(CONFIG_SLAVE_CHIP_SPECIFIC_DEFINES),-D$(D))

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
all: $(BUILD_DIR) compile link chip_specific_post_build

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
	platforms/$(CONFIG_SLAVE_PLATFORM) \
	$(foreach C,$(PLATFORM_COMPONENTS),platforms/$(CONFIG_SLAVE_PLATFORM)/$(C)) \
	$(foreach C,$(CHIP_COMPONENTS),chips/$(CONFIG_SLAVE_CHIP)/$(C))

$(BUILD_DIR): $(foreach D,$(BUILD_DIR_CONTENTS),$(BUILD_DIR)/$(D))
	mkdir -p $@/out

$(BUILD_DIR)/%:
	mkdir -p $@

# -------------------------------------------------------------------------

PHONY += clean
clean:
	rm -rf $(BUILD_DIR)

# -------------------------------------------------------------------------

include ../config/slave/chips/$(CONFIG_SLAVE_CHIP)/post_build.mk

PHONY += chip_specific_post_build

# -------------------------------------------------------------------------

.PHONY: $(PHONY)
