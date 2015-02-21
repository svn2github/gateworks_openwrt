# we use shell expansion below so require bash
SHELL=/bin/bash

# Directory name for openwrt tree
OPENWRT=trunk

# System-wide download directory
DL_DIR=/usr/src/dl

# Make arguments (use num of cpu cores as job count)
MAKEARGS=-j$(shell grep processor /proc/cpuinfo | wc -l)

# Build bootloaders (instead of fetching pre-built from Gateworks)
#BUILD_BOOTLOADERS=1

all: imx61 cns3xxx ixp4xx

# Fetch and prepare OpenWRT buildroot
${OPENWRT}:
	@./scripts/prepare ${OPENWRT} .config
	# create dirs
	@if [ "${DL_DIR}" -a -d "${DL_DIR}" ]; then \
		ln -s ${DL_DIR} ${OPENWRT}/dl; \
	fi
	@mkdir -p logs

# Build for Ventana family
imx61: ${OPENWRT}
	@if [ -r "config/$@/.config${PROFILE}" ]; then \
		echo "Building $@"; \
		cp config/$@/.config${PROFILE} ${OPENWRT}/.config; \
		make -C ${OPENWRT} ${MAKEARGS} V=99 2>&1 | tee logs/build_$@.log; \
	fi

# Build for Laguna family
cns3xxx: ${OPENWRT}
	@if [ -r "config/$@/.config${PROFILE}" ]; then \
		echo "Building $@"; \
		cp config/$@/.config${PROFILE} ${OPENWRT}/.config; \
		make -C ${OPENWRT} ${MAKEARGS} V=99 2>&1 | tee logs/build_$@.log; \
	fi

# Build for Avila/Cambria family
ixp4xx: ${OPENWRT}
	@if [ -r "config/$@/.config${PROFILE}" ]; then \
		echo "Building $@"; \
		cp config/$@/.config${PROFILE} ${OPENWRT}/.config; \
		make -C ${OPENWRT} ${MAKEARGS} V=99 2>&1 | tee logs/build_$@.log; \
	fi

ifdef BUILD_BOOTLOADERS
# Build laguna family bootloaders
bootloader/laguna:
	@echo "Fetching source for $@..."
	mkdir -p bootloader
	svn co http://svn.gateworks.com/laguna/u-boot-2008.10 $@
bootloader/laguna/u-boot_nor.bin: bootloader/laguna
	@echo "Building $@..."
	( cd bootloader/laguna; \
		export STAGING_DIR=$$PATH:$$PWD/../../${OPENWRT}/staging_dir; \
		PATH=$$STAGING_DIR/toolchain-arm_v6k_gcc-4.6-linaro_uClibc-0.9.33.2_eabi/bin; \
		export CROSS_COMPILE=arm-openwrt-linux-; \
		make cavium_cns3000_config; \
		make ${MAKEARGS}; \
		cp u-boot.bin u-boot_nor.bin; \
	)
bootloader/laguna/u-boot_spi.bin: bootloader/laguna
	@echo "Building $@..."
	( cd bootloader/laguna; \
		export STAGING_DIR=$$PATH:$$PWD/../../${OPENWRT}/staging_dir; \
		PATH=$$STAGING_DIR/toolchain-arm_v6k_gcc-4.6-linaro_uClibc-0.9.33.2_eabi/bin; \
		make cavium_cns3000_spi_config; \
		export CROSS_COMPILE=arm-openwrt-linux-; \
		make ${MAKEARGS}; \
		cp u-boot.bin u-boot_spi.bin; \
	)

else

# pre-built bootloaders 
.PHONY:bootloader/laguna/u-boot_nor.bin
bootloader/laguna/u-boot_nor.bin:
	rm -f bootloader/laguna/u-boot_nor.bin
	@echo "Fetching $@..."
	mkdir -p bootloader/laguna
	svn export http://svn.gateworks.com/laguna/images/u-boot_nor.bin \
			bootloader/laguna/u-boot_nor.bin

.PHONY:bootloader/laguna/u-boot_spi.bin
bootloader/laguna/u-boot_spi.bin:
	rm -f bootloader/laguna/u-boot_spi.bin
	@echo "Fetching $@..."
	mkdir -p bootloader/laguna
	svn export http://svn.gateworks.com/laguna/images/u-boot_spi.bin \
			bootloader/laguna/u-boot_spi.bin
endif

# firmware images
.PHONY:images
images: images/laguna

# firmware images for Laguna family
.PHONY:images/laguna
images/laguna: bootloader/laguna/u-boot_spi.bin bootloader/laguna/u-boot_nor.bin
	mkdir -p images
	(cd images; \
		../scripts/makeimage laguna_nor \
			../bootloader/laguna/u-boot_nor.bin ../${OPENWRT}; \
		../scripts/makeimage laguna_spi \
			../bootloader/laguna/u-boot_spi.bin ../${OPENWRT}; \
	)

# clean all
.PHONY:dirclean
dirclean:
	echo "Cleaning..."
	rm -rf ${OPENWRT}
	rm -rf bootloader
	rm -rf images
	rm -rf logs
	rm -rf tmp
