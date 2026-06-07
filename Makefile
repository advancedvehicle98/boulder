include ./config/*.mk

export BUILD_DIR = build

TGT_SLAVE = slave
TGT_MASTER = master

export MASTER_EXE = $(BUILD_DIR)/out/$(TGT_MASTER)
export SLAVE_EXE = $(BUILD_DIR)/out/$(TGT_SLAVE)

TARGETS = \
	$(TGT_MASTER) \
	$(TGT_SLAVE)

_TARGETS = $(foreach T,$(TARGETS),$(T).tgt)

TARGETS_CLEAN = $(foreach T,$(TARGETS),$(T).clean)

INCLUDE_DIRS = ./dsdl/include ./include
export CFLAGS += \
	$(foreach D,$(INCLUDE_DIRS),-I$(abspath $(D))) \
	-Wall

PHONY = all
all: build_dir dsdl $(_TARGETS)

%.tgt: %
	$(MAKE) -C ./$^
	cp ./$^/$(BUILD_DIR)/out/$^ $(BUILD_DIR)/out/$^

PHONY += build_dir
build_dir:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/out/$(TGT_MASTER)
	mkdir -p $(BUILD_DIR)/out/$(TGT_SLAVE)


# генерация файлов для dronecan/uavcan ------------------------------------

# надо чтобы пользователь был в venv в котором установлен dsdl 
DSDL_PY = ./modules/dronecan_dsdlc/dronecan_dsdlc.py
export DSDL_DIR = $(abspath ./dsdl)

# ЭТО НЕЛЬЗЯ ДОБАВЛЯТЬ В PHONY
dsdl: modules/DSDL
	$(DSDL_PY) -O $(DSDL_DIR) \
		./modules/DSDL/uavcan/ \
		./modules/DSDL/dronecan/

modules/DSDL:
	git submodule update --init --recursive


# то, о чем вежливо просил маслаев епифанцева в художественном к/ф "зелёный слоник" -----------------

PHONY += clean
clean: $(TARGETS_CLEAN) remove_squiggles
	rm -rf $(BUILD_DIR)
	rm -rf $(DSDL_DIR)

PHONY += $(TARGETS_CLEAN)
$(TARGETS_CLEAN):
	make -C $(@:%.clean=./%) clean

PHONY += remove_squiggles
remove_squiggles:
	find ./ | grep '~' | xargs rm -f


# удалённая установка ---------------------------------------------------------

RSYNC_SRC = $(BUILD_DIR)/out/$(TGT_MASTER)
RSYNC_DEST = $(CONFIG_MASTER_RSYNC_DEST):$(CONFIG_MASTER_RSYNC_INSTALL_DIR)

PHONY += install
install: 
	rsync -a $(RSYNC_SRC) $(RSYNC_DEST)


# прошивка (chip_specific_flash определяется в конфигах под определённый камень) --------------------

include config/slave/chips/$(CONFIG_SLAVE_CHIP)/flash.mk

PHONY += flash chip_specific_flash
flash: chip_specific_flash


# очень полезная инфа -----------------------------------

PHONY += print_master_platforms
print_master_platforms:
	@echo "Доступные платформы: "
	@echo $(foreach P,$(MASTER_PLATFORM_LIST),"\t$(P)\n")

PHONY += print_slave_platforms
print_slave_platforms:
	@echo "Доступные платформы: "
	@echo $(foreach P,$(SLAVE_PLATFORM_LIST),"\t$(P)\n")

PHONY += print_slave_chips
print_slave_chips:
	@echo "Доступные микроконтроллеры: "
	@echo $(foreach P,$(SLAVE_CHIP_LIST),"\t$(P)\n")


.PHONY: $(PHONY)
