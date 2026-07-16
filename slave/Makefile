include $(config)


# ==============================================================

ifeq ($(build-for-tests),y)
	objtree := $(objtree)/tests
endif

local         := $(srctree)/$(slave-prefix)
local-srctree := $(local)/src
prefix        := $(slave-prefix)

target := $(objtree)/$(prefix)/slave


# ==============================================================

# желательно префиксы расписывать в алфавитном порядке

dronecan-prefix := dronecan
dronecan-objs-y := handle_get_node_info.o

can-prefix := can
can-objs-y := init.o on_receive.o process_messages.o respond.o should_accept_transfer.o \
	$(addprefix $(dronecan-prefix)/, $(dronecan-objs-y))

ahrs-prefix := ahrs
ahrs-objs-y := init.o update.o

motors-prefix := motors
motors-objs-y := init.o update.o

queue-prefix := queue
queue-objs-y := init.o pop.o push.o ring_init.o ring_pop.o ring_push.o

ram-prefix := ram
ram-objs-y := copy.o format_string.o

range-prefix := range
range-objs-y := init.o update.o

uart-prefix := uart
uart-objs-y := init.o printf.o transmit.o

objs += handle_exit.o init.o main.o run.o \
	$(addprefix $(can-prefix)/, $(can-objs-y)) \
	$(addprefix $(ahrs-prefix)/, $(ahrs-objs-y)) \
	$(addprefix $(motors-prefix)/, $(motors-objs-y)) \
	$(addprefix $(queue-prefix)/, $(queue-objs-y)) \
	$(addprefix $(ram-prefix)/, $(ram-objs-y)) \
	$(addprefix $(range-prefix)/, $(range-objs-y)) \
	$(addprefix $(uart-prefix)/, $(uart-objs-y))

# -------------------------------------------------------------

ifeq ($(build-for-tests),y)
	CONFIG_SLAVE_BOARD_VENDOR := "dummy"
	CONFIG_SLAVE_BOARD_PREFIX := "dummy"

	CONFIG_SLAVE_SOC_VENDOR := "dummy"
	CONFIG_SLAVE_SOC_PREFIX := "dummy"

	CONFIG_SLAVE_ARCH_PREFIX := "dummy"
	CONFIG_SLAVE_ARCH_SUFFIX := "dummy"
endif

board-vendor := $(patsubst "%",%,$(CONFIG_SLAVE_BOARD_VENDOR))
board-prefix := $(patsubst "%",%,$(CONFIG_SLAVE_BOARD_PREFIX))

board-complete-prefix := $(board-vendor)/$(board-prefix)

soc-vendor := $(patsubst "%",%,$(CONFIG_SLAVE_SOC_VENDOR))
soc-prefix := $(patsubst "%",%,$(CONFIG_SLAVE_SOC_PREFIX))

soc-complete-prefix := $(soc-vendor)/$(soc-prefix)

arch-prefix := $(patsubst "%",%,$(CONFIG_SLAVE_ARCH_PREFIX))
arch-suffix := $(patsubst "%",%,$(CONFIG_SLAVE_ARCH_SUFFIX))

arch-complete-prefix := $(arch-prefix)/$(arch-suffix)

boardtree := $(local)/boards/$(board-complete-prefix)
soctree   := $(local)/soc/$(soc-complete-prefix)
archtree  := $(local)/arch/$(arch-complete-prefix)

# здесь и далее в подобных переменных строго плюс-равно т.к
# из Makefile.soc могут быть добавлены специфичные для
# платформы файлы

board-objs += init.o

# -----------------------------------------

soc-uart-prefix := uart
soc-uart-objs-y += init.o

soc-can-prefix := can
soc-can-objs-y += init.o

soc-objs += init.o \
	$(addprefix $(soc-can-prefix)/, $(soc-can-objs-y)) \
	$(addprefix $(soc-uart-prefix)/, $(soc-uart-objs-y))

# -----------------------------------------

arch-math-prefix := math
arch-math-objs-y += divmod10.o

arch-time-prefix := time
arch-time-objs-y += delay_ticks.o

arch-objs += init.o \
	$(addprefix $(arch-math-prefix)/, $(arch-math-objs-y)) \
	$(addprefix $(arch-time-prefix)/, $(arch-time-objs-y))

# -----------------------------------------

objs += \
	$(addprefix soc/$(soc-complete-prefix)/, $(soc-objs)) \
	$(addprefix boards/$(board-complete-prefix)/, $(board-objs)) \
	$(addprefix arch/$(arch-complete-prefix)/, $(arch-objs)) \
	$(addprefix dsdl/, $(dsdl-objs))

# --------------------------------------------

ldflags += \
	-Wl,--check-sections \
	-Wl,--gc-sections \
	-Wl,--unresolved-symbols=report-all \
	-Wl,--warn-common \
	-Wl,--warn-section-align

ifneq ($(build-for-tests),y)
cflags  += -nostdlib --specs=nosys.specs
ldflags += --specs=nosys.specs
endif

# --------------------------------------------

# soc-prerequisites и soc-post-build должны быть обределены в Makefile.soc

all-deps := objtree-prefixes soc-prerequisites $(target) soc-post-build

ifeq ($(build-for-tests),y)
all-deps := objtree-prefixes $(addprefix $(objtree)/$(prefix)/, $(objs))
endif

phony += all
all: $(all-deps)

# --------------------------------------------

include $(scripts)/make/Makefile.canard

# --------------------------------------------

ifneq ($(build-for-tests),y)

ifeq ($(CONFIG_SLAVE_TOOLCHAIN),)

ifeq ($(CONFIG_SLAVE_TOOLCHAIN_CC_PATH),)
	$(error Не указан путь к компилятору (см. CONFIG_SLAVE_TOOLCHAIN_CC_PATH в menuconfig))
endif

ifeq ($(CONFIG_SLAVE_TOOLCHAIN_OBJCOPY_PATH),)
	$(error Не указан путь к objcopy (см. CONFIG_SLAVE_TOOLCHAIN_OBJCOPY_PATH в menuconfig))
endif

cc    := $(CONFIG_SLAVE_TOOLCHAIN_CC_PATH)
objcp := $(CONFIG_SLAVE_TOOLCHAIN_OBJCOPY_PATH)

ifeq ($(CONFIG_SLAVE_TOOLCHAIN_ASM_PATH),)
	$(warning Не указан путь к ассемблеру. Предпололим, что $(cc) - это ассемблер)
	asm := $(cc)
else
	asm := $(CONFIG_SLAVE_TOOLCHAIN_ASM_PATH)
endif

ifeq ($(CONFIG_SLAVE_TOOLCHAIN_LD_PATH),)
	$(warning Не указан путь к линковщику. Предпололим, что $(cc) - это линковщик)
	ld := $(cc)
else
	ld := $(CONFIG_SLAVE_TOOLCHAIN_LD_PATH)
endif

else # ifneq ($(CONFIG_SLAVE_TOOLCHAIN),)

toolchain = $(patsubst "%",%,$(CONFIG_SLAVE_TOOLCHAIN))-

asm   := $(toolchain)$(asm)
cc    := $(toolchain)$(cc)
ld    := $(toolchain)$(ld)
objcp := $(toolchain)$(objcp)

endif # ifeq ($(CONFIG_SLAVE_TOOLCHAIN),)

endif # ifneq ($(build-for-tests),y)

# на всякий
export cc ld asm objcp

# -----------------------------

soc-firmware := $(objtree)/$(prefix)/slave.hex

-include $(soctree)/Makefile.soc

# т.е. в каждом soc-е должен быть свой include, в котором есть soc, в котором есть перечень
# обязательных заголовков
board-include := $(soctree)/include $(boardtree)/include $(archtree)/include
board-include := $(addprefix -I, $(board-include))

cflags  += $(board-include)
asflags += $(board-include)

# -------------------------------------------------------------

inc-paths := \
	$(local)/include \
	$(dsdl-path)/include

# сюда могли быть добавлены дополнительные флаги из Makefile.soc
cflags += \
	$(addprefix -I,$(inc-paths)) \
	-std=gnu99 \
	-ffunction-sections \
	-fdata-sections \
	-Wno-main

ldflags += 

libs += $(soc-libs)

# -------------------------------------------------------------

include $(scripts)/make/Makefile.build

# -------------------------------------------------------------

soc-objtree := $(objtree)/slave/soc/$(soc-complete-prefix)

$(soc-objtree)/%.o: $(local)/soc/$(soc-complete-prefix)/%.c
	$(q)$(cc) $(cflags) -c $^ -o $@

soc-objtree/%.o: $(local)/soc/$(soc-complete-prefix)/%.S
	$(q)$(asm) $(asflags) -c $^ -o $@

# -------------------------------------------------------------

arch-objtree := $(objtree)/slave/arch/$(arch-complete-prefix)

$(arch-objtree)/%.o: $(local)/arch/$(arch-complete-prefix)/%.c
	$(q)$(cc) $(cflags) -c $^ -o $@

$(arch-objtree)/%.o: $(local)/arch/$(arch-complete-prefix)/%.S
	$(q)$(asm) $(asflags) -c $^ -o $@

# -------------------------------------------------------------

board-objtree := $(objtree)/slave/boards/$(board-complete-prefix)

$(board-objtree)/%.o: $(local)/boards/$(board-complete-prefix)/%.c
	$(q)$(cc) $(cflags) -c $^ -o $@

$(board-objtree)/%.o: $(local)/boards/$(board-complete-prefix)/%.S
	$(q)$(asm) $(asflags) -c $^ -o $@


# ==============================================================

.PHONY: $(phony)
