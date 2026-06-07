BOSSAC_FLAGS = \
	--port $(CONFIG_SLAVE_UPLOAD_PORT) \
	--write \
	--verify \
	--reset \
	--erase \
	--usb-port \
	--boot \
	--info

chip_specific_flash:
	bossac $(BOSSAC_FLAGS) $(SLAVE_EXE)
