# платформа (под чё собирать, см. что есть в директории master/platform)
# пока сегрегации тонкой сегрегации нет, но она наверное и не нужна будет
export CONFIG_MASTER_PLATFORM ?= repka-pi3


# rsync
CONFIG_MASTER_RSYNC_INSTALL_DIR = /opt/boulder
CONFIG_MASTER_RSYNC_DEST = root@192.168.3.11


# ДАЛЬШЕ НИЧЕ НЕЛЬЗЯ ТРОГАТЬ ---------------------------------------------

include config/master/platforms/$(CONFIG_MASTER_PLATFORM)/config.mk


export MASTER_PLATFORM_LIST = \
	native \
	repka-pi3
