chip_specific_post_build: 
	$(TOOLCHAIN)-objcopy -O binary $(BUILD_DIR)/out/slave.elf $(BUILD_DIR)/out/slave
