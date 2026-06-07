export CONFIG_SLAVE_TOOLCHAIN = arm-none-eabi


export CONFIG_SLAVE_CFLAGS = \
	-mcpu=cortex-m3 \
	-mthumb \
	--specs=nosys.specs \
	-Dprintf=iprintf \
	--param max-inline-insns-single=500 \
	-mlong-calls


export CONFIG_SLAVE_LDFLAGS = \
	-mcpu=cortex-m3 \
	-mthumb \
	--specs=nosys.specs


export CONFIG_SLAVE_UPLOAD_PORT = /dev/ttyACM0


export CONFIG_SLAVE_CAN_IFACE = CAN1

export CONFIG_SLAVE_CHIP_SPECIFIC_DEFINES = \
	CONFIG_SLAVE_CAN_IFACE_$(CONFIG_SLAVE_CAN_IFACE)
