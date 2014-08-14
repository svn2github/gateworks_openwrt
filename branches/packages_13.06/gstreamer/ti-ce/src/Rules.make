# This make variable must be set before the DVSDK components can be built.
PLATFORM=$(PLATFORM)
ifndef PLATFORM
   $(error PLATFORM must be set in Rules.make to dm355, dm6467 or dm6446 before building)
endif


# The installation directory of the DVSDK
DVSDK_INSTALL_DIR=/usr/src/davinci/work

# For backwards compatibility
DVEVM_INSTALL_DIR=$(DVSDK_INSTALL_DIR)

# Where the DVSDK demos are installed
DEMO_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/dvsdk_demos_2_00_00_07

# Where the Digital Video Test Bench is installed
DVTB_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/dvtb_4_00_08

# Where the Davinci Multimedia Application Interface is installed
DMAI_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/davinci_multimedia_application_interface/dmai

# Where the Codec Engine package is installed.
CE_INSTALL_DIR=$(DVSDK_INSTALL_DIR)

# Where the XDAIS package is installed.
XDAIS_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/cetools

# Where the DSP Link package is installed.
LINK_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/ti-dsplink-module

# Where the CMEM (contiguous memory allocator) package is installed.
CMEM_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/cetools

# Where the EDMA3 Low Level Driver is installed.
EDMA3_LLD_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/edma3_lld_1_05_00

ifeq ($(PLATFORM),dm6467)
   CODEC_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/dm6467_dvsdk_combos_2_05
endif
ifeq ($(PLATFORM),dm6446)
   CODEC_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/dm6446_dvsdk_combos_2_05
endif
ifeq ($(PLATFORM),dm355)
   CODEC_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/dm355_codecs_1_13_000
endif

# Where the RTSC tools package is installed.
XDC_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/ti-xdctools-native

# Where Framework Components product is installed
FC_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/cetools

# Where DSP/BIOS is installed
BIOS_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/ti-dspbios-native

# BIOS Utilities
BIOSUTILS_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/biosutils_1_01_00

# Additional RTSC package repositories to be picked up by components.
USER_XDC_PATH=$(CE_INSTALL_DIR)/examples
#above works around DMAI path bug

# Where the TI c6x code generation tools are installed
CODEGEN_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/ti-cgt6x-native

# Platform Support Package installation directory.
PSP_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/PSP_02_00_00_140

# The directory that points to your kernel source directory.
LINUXKERNEL_INSTALL_DIR=/usr/src/openwrt/backfire/build_dir/linux-davinci/linux-2.6.32.10

# The prefix to be added before the GNU compiler tools (optionally including
# path), i.e. "arm_v5t_le-" or "/opt/bin/arm_v5t_le-".
MVTOOL_DIR=/usr/src/openwrt/backfire/staging_dir/toolchain-arm10_v5t_gcc-4.3.3+cs_uClibc-0.9.30.1_eabi/usr
MVTOOL_PREFIX=$(MVTOOL_DIR)/bin/arm-openwrt-linux-

# Where to copy the resulting executables and data to (when executing 'make
# install') in a proper file structure. This EXEC_DIR should either be visible
# from the target, or you will have to copy this (whole) directory onto the
# target filesystem.
EXEC_DIR=$(DVSDK_INSTALL_DIR)/install/$(PLATFORM)

LINUXLIBS_INSTALL_DIR=/usr/src/openwrt/backfire/staging_dir/toolchain-arm10_v5t_gcc-4.3.3+cs_uClibc-0.9.30.1_eabi/usr

CROSS_COMPILE = \
    /usr/src/openwrt/backfire/staging_dir/toolchain-arm10_v5t_gcc-4.3.3+cs_uClibc-0.9.30.1_eabi/usr/bin/arm-openwrt-linux-

CC=$(MVTOOL_PREFIX)gcc
AR=$(MVTOOL_PREFIX)ar
LD=$(MVTOOL_PREFIX)ld
COMPILER=$(CC)
ARCHIVER=$(AR)
KERNEL_DIR=$(LINUXKERNEL_INSTALL_DIR)
OBJDUMP=$(MVTOOL_PREFIX)objdump
