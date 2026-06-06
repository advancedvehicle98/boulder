export CONFIG_SLAVE_TOOLCHAIN = arm-none-eabi

export CONFIG_SLAVE_CFLAGS = \
	-mcpu=cortex-m3 \
	-mthumb \
	--specs=nosys.specs

export CONFIG_SLAVE_LDFLAGS = \
	-mcpu=cortex-m3 \
	-mthumb \
	--specs=nosys.specs \
	-Dprintf=iprintf \
	--param max-inline-insns-single=500 \
	-mlong-calls
