# платформа (под чё собирать, см. что есть в директории slave/platform)
export CONFIG_SLAVE_PLATFORM ?= studica-blackhawk


include config/slave/platforms/$(CONFIG_SLAVE_PLATFORM)/config.mk

include config/slave/chips/$(CONFIG_SLAVE_CHIP)/config.mk


export SLAVE_PLATFORM_LIST = \
	studica-blackhawk

export SLAVE_CHIP_LIST = \
	sam3x8e
