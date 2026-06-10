include $(config)


# ==============================================================

local         := $(srctree)/$(slave-prefix)
local-srctree := $(local)/src
prefix        := $(slave-prefix)

target := $(objtree)/$(prefix)/slave


# ==============================================================

# soc-prerequisites должен быть обределён в Makefile.soc

phony += all
all: objtree-prefixes soc-prerequisites $(target)


# ==============================================================

can-prefix := can
can-objs-y := init.o

objs += handle_exit.o init.o main.o run.o \
	$(addprefix $(can-prefix)/, $(can-objs-y))

# --------------------------------------------

ifeq ($(CONFIG_SLAVE_TOOLCHAIN),)

cc := $(CONFIG_SLAVE_TOOLCHAIN_CC_PATH)
ld := $(CONFIG_SLAVE_TOOLCHAIN_LD_PATH)

else

toolchain = $(CONFIG_SLAVE_TOOLCHAIN)-

cc := $(toolchain)$(cc)
ld := $(toolchain)$(ld)

endif

export cc ld

# -------------------------------------------------------------

board-vendor := $(patsubst "%",%,$(CONFIG_SLAVE_BOARD_VENDOR))
board-prefix := $(patsubst "%",%,$(CONFIG_SLAVE_BOARD_PREFIX))

board-complete-prefix := $(board-vendor)/$(board-prefix)

soc-vendor := $(patsubst "%",%,$(CONFIG_SLAVE_SOC_VENDOR))
soc-prefix := $(patsubst "%",%,$(CONFIG_SLAVE_SOC_PREFIX))

soc-complete-prefix := $(soc-vendor)/$(soc-prefix)

arch-prefix := $(patsubst "%",%,$(CONFIG_SLAVE_ARCH_PREFIX))
arch-suffix := $(patsubst "%",%,$(CONFIG_SLAVE_ARCH_SUFFIX))

arch-complete-prefix := $(arch-prefix)/$(arch-suffix)

boardtree := $(local)/board/$(board-complete-prefix)
soctree   := $(local)/soc/$(soc-complete-prefix)
archtree  := $(local)/arch/$(arch-complete-prefix)

include $(soctree)/Makefile.soc

board-objs := init.o

soc-can-prefix := can
soc-can-obj-y  := init.o

soc-objs := init.o \
	$(addprefix $(soc-can-prefix)/, $(soc-can-obj-y))

objs += \
	$(addprefix soc/$(soc-complete-prefix)/, $(soc-objs)) \
	$(addprefix boards/$(board-complete-prefix)/, $(board-objs))

# т.е. в каждом soc-е должен быть свой include, в котором есть soc, в котором есть перечень
# обязательных заголовков
board-include := $(soctree)/include
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
	-fdata-sections

ldflags += 

libs += $(soc-libs)

# -------------------------------------------------------------

include $(scripts)/make/Makefile.build
# это если для soc надо собирать бинарники, исходные
# файлы которых лежат не по префиксам
-include $(scripts)/make/Makefile.soc.build

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
