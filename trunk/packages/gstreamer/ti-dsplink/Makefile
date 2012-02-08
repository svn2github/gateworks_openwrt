include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=ti-dsplink
#PKG_VERSION:=1_65_00_03
#PKG_MD5SUM:=1bda596b631bd2f517edc70f6be4f2ca
PKG_VERSION:=1_65_00_02
PKG_MD5SUM:=e05b1f2b2b4a1a92c15746e9c541a80f

PKG_SOURCE:=dsplink_linux_$(PKG_VERSION).tar.gz
PKG_BUILD_DIR:=$(TOPDIR)/build_dir/target/dsplink_linux_$(PKG_VERSION)
PKG_SOURCE_URL:=http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/DSPLink/$(PKG_VERSION)/

include $(INCLUDE_DIR)/package.mk

define KernelPackage/ti-dsplink
	SECTION:=local
	CATEGORY:=Local
	TITLE:=TI DSP-Link module for communcations with the DSP
	URL:=http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/DSPLink/
	DEPENDS:=@TARGET_davinci
	FILES:=$(PKG_BUILD_DIR)/kernel_binaries/dm6446/dsplinkk.ko
endef

define Package/ti-dsplink
	SECTION:=local
	CATEGORY:=Local
	TITLE:=TI DSP BIOS
	DEPENDS:=+ti-xdc +ti-cg +ti-dspbios
endef

define Package/ti-dsplink/description
	TI DSPLink provides linkage between the General Purpose Processor (GPP) and Digital Signal Processor (DSP)
endef

define Build/Prepare
	# deal with retarded TI requirement that path not contain "." by symlinking
	# "target-arm_v5t_uClibc-0.9.30.1_eabi" to "target".  Do it carefully
	# so that re-making doesn't break.
	rm -fr $(TOPDIR)/build_dir/target
	ln -s $(BUILD_DIR) $(TOPDIR)/build_dir/target
	rm -fr $(PKG_BUILD_DIR)
	mkdir $(PKG_BUILD_DIR)
	# Unpack tar file
	$(call Build/Prepare/Default)
	$(CP) ./src/* $(PKG_BUILD_DIR)/	
	(cd $(PKG_BUILD_DIR); mkdir packages; mv dsplink packages )
	# create a generic symlink to package so that other TI packages can find it
	rm -f $(BUILD_DIR)/$(PKG_NAME)
	ln -s $(PKG_BUILD_DIR) $(BUILD_DIR)/$(PKG_NAME)
endef

DVSDK_PATHS = \
	DVSDK_INSTALL_DIR=$(PKG_BUILD_DIR) \
	LINK_INSTALL_DIR=$(PKG_BUILD_DIR) \
	XDC_INSTALL_DIR=$(BUILD_DIR)/ti-xdc \
	BIOS_INSTALL_DIR=$(BUILD_DIR)/ti-dspbios \
	CODEGEN_INSTALL_DIR=$(BUILD_DIR)/ti-cg \
	KERNEL_DIR=$(LINUX_DIR) \
	LINUXKERNEL_INSTALL_DIR=$(LINUX_DIR) \
	MVTOOL_DIR=$(TOOLCHAIN_DIR) \

define Build/Compile
	# Build dsplink, including the GPP (general-purpose processor) side, the
	# DSP side, and the sample programs.
	$(MAKE) -C $(PKG_BUILD_DIR) \
		PLATFORM=dm6446 \
		$(DVSDK_PATHS) \
		CROSS_COMPILE=$(TARGET_CROSS) \
		dsplink

	# Enable XDC-based integration (as per
	# http://processors.wiki.ti.com/index.php/Building_DSPLink )
	(cd $(PKG_BUILD_DIR)/packages/dsplink/dsp; $(BUILD_DIR)/ti-xdc/xdc clean; $(BUILD_DIR)/ti-xdc/xdc .interfaces )
	(cd $(PKG_BUILD_DIR)/packages/dsplink/gpp; $(BUILD_DIR)/ti-xdc/xdc clean; $(BUILD_DIR)/ti-xdc/xdc .interfaces )
endef

define Build/Clean
	rm -f $(BUILD_DIR)/$(PKG_NAME)
endef

define Package/ti-dsplink/install
	/bin/true
endef

$(eval $(call BuildPackage,ti-dsplink))
$(eval $(call KernelPackage,ti-dsplink))
