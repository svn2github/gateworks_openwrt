# we use shell expansion below so require bash
SHELL=/bin/bash

# OpenWRT and external package feed src revision
# last updated 2012-04-16
SRCREV_openwrt=27590
# list of pinned feeds: <feedname>:<feedurl>[:svnrevision]
FEEDS = \
	gateworks;http://svn.gateworks.com/openwrt/trunk/packages;398 \
	luci;http://svn.luci.subsignal.org/luci/trunk/contrib/package;7265 \
	packages;svn://svn.openwrt.org/openwrt/packages;${SRCREV_openwrt} \
	xwrt;http://x-wrt.googlecode.com/svn/trunk/package;4986

# list of other pinnings
PINNINGS = \
	feeds/packages/libs/libv4l;32499 \

# Directory name for openwrt tree
OPENWRT=trunk

# System-wide download directory
DL_DIR=/usr/src/dl

# Make arguments (use num of cpu cores as job count)
MAKEARGS=-j$(shell grep processor /proc/cpuinfo | wc -l)

# Build bootloaders (instead of fetching pre-built from Gateworks)
#BUILD_BOOTLOADERS=1

all: cns3xxx ixp4xx davinci

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
	    svn checkout -r$(rev) $(url) ${OPENWRT}/feeds/$(name) || exit 1; \
	)
	@echo "Fetching pinned dirs..."
	@$(foreach pin, $(PINNINGS), \
	    $(eval dir = $(word 1,$(subst ;, ,$(pin)))) \
	    $(eval rev = $(or $(word 2,$(subst ;, ,$(pin))),HEAD)) \
	    \
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

# Build for Laguna family
cns3xxx: ${OPENWRT}
	@echo "Building $@"
	cp config/$@/.config${PROFILE} ${OPENWRT}/.config
	time make -C ${OPENWRT} ${MAKEARGS} V=99 2>&1 | tee logs/build_$@.log

# Build for Rincon family
davinci: ${OPENWRT}
	@echo "Building $@"
	cp config/$@/.config${PROFILE} ${OPENWRT}/.config
	time make -C ${OPENWRT} ${MAKEARGS} V=99 2>&1 | tee logs/build_$@.log

# Build for Avila/Cambria family
ixp4xx: ${OPENWRT}
	@echo "Building $@"
	cp config/$@/.config${PROFILE} ${OPENWRT}/.config
	time make -C ${OPENWRT} ${MAKEARGS} V=99 2>&1 | tee logs/build_$@.log

ifdef BUILD_BOOTLOADERS
# Build laguna family bootloaders
bootloader/laguna:
	@echo "Fetching source for $@..."
	mkdir -p bootloader
	svn co http://svn.gateworks.com/laguna/trunk/u-boot-2008.10 $@
bootloader/laguna/u-boot_nor.bin: bootloader/laguna
	@echo "Building $@..."
	( cd bootloader/laguna; \
		PATH=${PATH}:${PWD}/${OPENWRT}/staging_dir/toolchain-arm_v6k_gcc-linaro_uClibc-0.9.32_eabi/bin; \
		make cavium_cns3000_config; \
		make; \
		cp u-boot.bin u-boot_nor.bin; \
	)
bootloader/laguna/u-boot_spi.bin: bootloader/laguna
	@echo "Building $@..."
	( cd bootloader/laguna; \
		PATH=${PATH}:${PWD}/${OPENWRT}/staging_dir/toolchain-arm_v6k_gcc-linaro_uClibc-0.9.32_eabi/bin; \
		make cavium_cns3000_spi_config; \
		make; \
		cp u-boot.bin u-boot_spi.bin; \
	)

# Build rincon family bootloaders
bootloader/rincon:
	@echo "Fetching source for $@..."
	mkdir -p bootloader
	svn co http://svn.gateworks.com/rincon/trunk/u-boot $@
bootloader/rincon/u-boot.bin: bootloader/rincon
	@echo "Building $@..."
	( cd bootloader/rincon; \
		PATH=${PATH}:${PWD}/${OPENWRT}/staging_dir/toolchain-arm_v5t_gcc-linaro_uClibc-0.9.32_eabi/bin; \
		make davinci_dvevm_config; \
		make; \
	)

else

# pre-built bootloaders 
.PHONY:bootloader/rincon/u-boot.bin
bootloader/rincon/u-boot.bin:
	rm -f bootloader/rincon/u-boot.bin
	@echo "Fetching $@..."
	mkdir -p bootloader/rincon
	svn export http://svn.gateworks.com/rincon/trunk/images/u-boot.bin \
			bootloader/rincon/u-boot.bin
.PHONY:bootloader/laguna/u-boot_nor.bin
bootloader/laguna/u-boot_nor.bin:
	rm -f bootloader/laguna/u-boot_nor.bin
	@echo "Fetching $@..."
	mkdir -p bootloader/laguna
	svn export http://svn.gateworks.com/laguna/trunk/images/u-boot_nor.bin \
			bootloader/laguna/u-boot_nor.bin
.PHONY:bootloader/laguna/u-boot_spi.bin
bootloader/laguna/u-boot_spi.bin:
	rm -f bootloader/laguna/u-boot_spi.bin
	@echo "Fetching $@..."
	mkdir -p bootloader/laguna
	svn export http://svn.gateworks.com/laguna/trunk/images/u-boot_spi.bin \
			bootloader/laguna/u-boot_spi.bin
endif

# firmware images
.PHONY:images
images: images/laguna images/rincon

# firmware images for Laguna family
.PHONY:images/laguna
images/laguna: bootloader/laguna/u-boot_spi.bin bootloader/laguna/u-boot_nor.bin
	mkdir -p images
	(cd images; \
		../scripts/makeimage gw2388 \
			../bootloader/laguna/u-boot_nor.bin ../${OPENWRT}; \
		../scripts/makeimage gw2380 \
			../bootloader/laguna/u-boot_spi.bin ../${OPENWRT}; \
	)

# firmware images for Rincon family
images/rincon: bootloader/rincon/u-boot.bin
	mkdir -p images
	(cd images; \
		../scripts/makeimage gw2361 \
			../bootloader/rincon/u-boot.bin ../${OPENWRT}; \
	)

# clean all
dirclean:
	echo "Cleaning..."
	rm -rf ${OPENWRT}
	rm -rf bootloader
	rm -rf images
	rm -rf logs
	rm -rf tmp
