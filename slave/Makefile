BUILD_DIR ?= ./build
SRC_DIR = ./src

COMPONENTS = 

TARGETS =

ALL_DIRS = \
	$(SRC_DIR) \
	$(foreach C,$(COMPONENTS),$(SRC_DIR)/$(C))


all: build_dir $(TARGETS)


build_dir:
	mkdir -p $(BUILD_DIR)


clean: remove_squiggles
	rm -rf $(BUILD_DIR)

remove_squiggles:
	rm -rf $(foreach D,$(ALL_DIRS),$(D)/*~)
