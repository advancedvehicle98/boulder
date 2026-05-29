include ./config/*.mk

export BUILD_DIR = build

TGT_SLAVE = slave
TGT_MASTER = master

TARGETS = \
	$(TGT_MASTER)#  \
	# $(TGT_SLAVE)

_TARGETS = $(foreach T,$(TARGETS),$(T).tgt)

TARGETS_CLEAN = $(foreach T,$(TARGETS),$(T).clean)

ALL_DIRS = . \
	./$(TGT_SLAVE) ./$(TGT_MASTER)


PHONY = all
all: build_dir $(_TARGETS)

%.tgt: %
	$(MAKE) -C ./$^
	cp ./$^/$(BUILD_DIR)/out/$^ ./$(BUILD_DIR)/out/$^

PHONY += build_dir
build_dir:
	mkdir -p $(BUILD_DIR)
	# mkdir -p $(BUILD_DIR)/out/$(TGT_SLAVE)
	mkdir -p $(BUILD_DIR)/out/$(TGT_MASTER)


PHONY += clean
clean: $(TARGETS_CLEAN) remove_squiggles
	rm -rf $(BUILD_DIR)

PHONY += $(TARGETS_CLEAN)
$(TARGETS_CLEAN):
	make -C $(@:%.clean=./%) clean

PHONY += remove_squiggles
remove_squiggles:
	rm -rf $(foreach D,$(ALL_DIRS),$(D)/*~)


RSYNC_SRC = $(BUILD_DIR)/out/$(TGT_MASTER)
RSYNC_DEST = $(CONFIG_MASTER_RSYNC_DEST):$(CONFIG_MASTER_RSYNC_INSTALL_DIR)

PHONY += install
install: 
	rsync -a $(RSYNC_SRC) $(RSYNC_DEST)

PHONY += flash
flash:
	$(CONFIG_SLAVE_FLASH_COMMAND)


.PHONY: $(PHONY)
