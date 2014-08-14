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

all: imx6 cns3xxx ixp4xx

# Fetch and prepare OpenWRT buildroot
${OPENWRT}:
	@./scripts/prepare ${OPENWRT} .config
	# create dirs
	@if [ "${DL_DIR}" -a -d "${DL_DIR}" ]; then \
		ln -s ${DL_DIR} ${OPENWRT}/dl; \
	fi
	@mkdir -p logs

# Build for Ventana family
imx6: ${OPENWRT}
	@if [ -r "config/$@/.config${PROFILE}" ]; then \
		echo "Building $@"; \
		cp config/$@/.config${PROFILE} ${OPENWRT}/.config; \
		time make -C ${OPENWRT} ${MAKEARGS} V=99 2>&1 | tee logs/build_$@.log; \
	fi

# Build for Laguna family
cns3xxx: ${OPENWRT}
	@if [ -r "config/$@/.config${PROFILE}" ]; then \
		echo "Building $@"; \
		cp config/$@/.config${PROFILE} ${OPENWRT}/.config; \
		time make -C ${OPENWRT} ${MAKEARGS} V=99 2>&1 | tee logs/build_$@.log; \
	fi

# Build for Avila/Cambria family
ixp4xx: ${OPENWRT}
	@if [ -r "config/$@/.config${PROFILE}" ]; then \
		echo "Building $@"; \
		cp config/$@/.config${PROFILE} ${OPENWRT}/.config; \
		time make -C ${OPENWRT} ${MAKEARGS} V=99 2>&1 | tee logs/build_$@.log; \
	fi

ifdef BUILD_BOOTLOADERS
# Build laguna family bootloaders
bootloader/ventana:
	@echo "Fetching source for $@..."
	mkdir -p bootloader
	git clone https://github.com/Gateworks/u-boot-imx6.git bootloader/ventana
bootloader/ventana/u-boot.img: bootloader/ventana
	@echo "Building $@..."
	( cd bootloader/ventana; \
		export STAGING_DIR=$$PATH:$$PWD/../../${OPENWRT}/staging_dir; \
		PATH=$$STAGING_DIR/toolchain-arm_cortex-a9+neon_gcc-4.6-linaro_uClibc-0.9.33.2_eabi/bin; \
		export CROSS_COMPILE=arm-openwrt-linux-; \
		make gw_ventana_config; \
		make ${MAKEARGS}; \
	)
bootloader/laguna:
	@echo "Fetching source for $@..."
	mkdir -p bootloader
	svn co http://svn.gateworks.com/laguna/u-boot-2008.10 $@
bootloader/laguna/u-boot_nor.bin: bootloader/laguna
	@echo "Building $@..."
	( cd bootloader/laguna; \
		export STAGING_DIR=$$PATH:$$PWD/../../${OPENWRT}/staging_dir; \
		PATH=$$STAGING_DIR/toolchain-arm_mpcore+vfp_gcc-4.6-linaro_uClibc-0.9.33.2_eabi/bin; \
		export CROSS_COMPILE=arm-openwrt-linux-; \
		make cavium_cns3000_config; \
		make ${MAKEARGS}; \
		cp u-boot.bin u-boot_nor.bin; \
	)
bootloader/laguna/u-boot_spi.bin: bootloader/laguna
	@echo "Building $@..."
	( cd bootloader/laguna; \
		export STAGING_DIR=$$PATH:$$PWD/../../${OPENWRT}/staging_dir; \
		PATH=$$STAGING_DIR/toolchain-arm_mpcore+vfp_gcc-4.6-linaro_uClibc-0.9.33.2_eabi/bin; \
		make cavium_cns3000_spi_config; \
		export CROSS_COMPILE=arm-openwrt-linux-; \
		make ${MAKEARGS}; \
		cp u-boot.bin u-boot_spi.bin; \
	)

else

# pre-built bootloaders 
.PHONY:bootloader/ventana/SPL
bootloader/ventana/SPL:
	rm -f bootloader/ventana/SPL
	@echo "Fetching $@..."
	mkdir -p bootloader/ventana
	svn export http://svn.gateworks.com/ventana/images/SPL \
			bootloader/ventana/SPL

.PHONY:bootloader/ventana/u-boot.img
bootloader/ventana/u-boot.img:
	rm -f bootloader/ventana/u-boot.img bootloader/ventana/SPL
	@echo "Fetching $@..."
	mkdir -p bootloader/ventana
	svn export http://svn.gateworks.com/ventana/images/u-boot.img \
			bootloader/ventana/u-boot.img
	svn export http://svn.gateworks.com/ventana/images/SPL \
			bootloader/ventana/SPL

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
images: images/ventana images/laguna

# JTAG firmware images for Ventana family
.PHONY:images/ventana
images/ventana: bootloader/ventana/SPL bootloader/ventana/u-boot.img
	mkdir -p images
	(cd images; \
		../scripts/mkimage_jtag \
			../bootloader/ventana/SPL \
			../bootloader/ventana/u-boot.img \
			> ../bootloader/ventana/u-boot_spl.bin; \
		../scripts/mkimage_jtag \
			../bootloader/ventana/SPL \
			../bootloader/ventana/u-boot.img \
			../${OPENWRT}/bin/imx6/openwrt-imx6-ventana-rootfs_normal.ubi > ventana_normal.bin; \
		../scripts/mkimage_jtag \
			../bootloader/ventana/SPL \
			../bootloader/ventana/u-boot.img \
			../${OPENWRT}/bin/imx6/openwrt-imx6-ventana-rootfs_large.ubi > ventana_large.bin; \
	)

# JTAG firmware images for Laguna family
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
