# we use shell expansion below so require bash
SHELL=/bin/bash

# OpenWRT and external package feed src revision
#Last Changed Date: 2013-04-10 14:17:33 -0700 (Wed, 10 Apr 2013)
SRCREV_openwrt=36311
SRCREV_luci=9809
# list of pinned feeds: <feedname>:<feedurl>[:svnrevision]
FEEDS = \
	gateworks;http://svn.gateworks.com/openwrt/branches/packages_13.06 \
	luci;http://svn.luci.subsignal.org/luci/trunk;${SRCREV_luci} \
	packages;svn://svn.openwrt.org/openwrt/packages;${SRCREV_openwrt}

# list of other pinnings
PINNINGS=

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
	@echo "Fetching Openwrt trunk@r${SRCREV_openwrt}..."
	svn co -r${SRCREV_openwrt} svn://svn.openwrt.org/openwrt/trunk ${OPENWRT}
	@echo "Patching..."
	./${OPENWRT}/scripts/patch-kernel.sh ${OPENWRT} ./patches
	@echo "Fetching pinned package feeds..."
	mkdir -p $(OPENWRT)/feeds
	@$(foreach feed, $(FEEDS), \
	    $(eval name = $(word 1,$(subst ;, ,$(feed)))) \
	    $(eval url  = $(word 2,$(subst ;, ,$(feed)))) \
	    $(eval rev  = $(or $(word 3,$(subst ;, ,$(feed))),HEAD)) \
	    \
	    echo "svn checkout -r$(rev) $(url) ${OPENWRT}/feeds/$(name)"; \
	    svn checkout -r$(rev) $(url) ${OPENWRT}/feeds/$(name) || exit 1; \
	)
	@echo "Fetching pinned dirs..."
	@$(foreach pin, $(PINNINGS), \
	    $(eval dir = $(word 1,$(subst ;, ,$(pin)))) \
	    $(eval rev = $(or $(word 2,$(subst ;, ,$(pin))),HEAD)) \
	    \
	    echo "svn update -r$(rev) ${OPENWRT}/$(dir)"; \
	    svn update -r$(rev) ${OPENWRT}/$(dir) || exit 1; \
	)
	@echo "Creating package symlinks..."
	make -C ${OPENWRT} defconfig >/dev/null # buildroot needs a valid config
	make -C ${OPENWRT} package/symlinks-install
	# create dirs
	@if [ "${DL_DIR}" -a -d "${DL_DIR}" ]; then \
		ln -s ${DL_DIR} ${OPENWRT}/dl; \
	fi
	mkdir -p logs

# Build for Ventana family
imx61: ${OPENWRT}
	@echo "Building $@"
	cp config/$@/.config${PROFILE} ${OPENWRT}/.config
	make -C ${OPENWRT} ${MAKEARGS} V=99 2>&1 | tee logs/build_$@.log

# Build for Laguna family
cns3xxx: ${OPENWRT}
	@echo "Building $@"
	cp config/$@/.config${PROFILE} ${OPENWRT}/.config
	make -C ${OPENWRT} ${MAKEARGS} V=99 2>&1 | tee logs/build_$@.log

# Build for Avila/Cambria family
ixp4xx: ${OPENWRT}
	@echo "Building $@"
	cp config/$@/.config${PROFILE} ${OPENWRT}/.config
	make -C ${OPENWRT} ${MAKEARGS} V=99 2>&1 | tee logs/build_$@.log

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
