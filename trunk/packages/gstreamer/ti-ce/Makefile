include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=ti-ce
# TODO: bump to latest
#PKG_VERSION:=3_21_02_25
#PKG_MD5SUM:=2d05c7201b45857e7cc5132a1244c874
# ti-codecs fails to build against 3_21_01_23
#PKG_VERSION:=3_21_01_23
#PKG_MD5SUM:=a9ef91124364e43e6fc1484287515096
PKG_VERSION:=2_26_02_11
PKG_MD5SUM:=3a877ea4f869a1bfb36d7a1998cbfd0c
#PKG_VERSION:=2_25_01_06
#PKG_MD5SUM:=af4a4e9ddfef668fccd37b2071a7729c

PKG_SOURCE:=codec_engine_$(PKG_VERSION).tar.gz
PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/codec_engine_$(PKG_VERSION)
PKG_SOURCE_URL:=http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/ce/$(PKG_VERSION)/exports/

include $(INCLUDE_DIR)/package.mk

define KernelPackage/ti-cmem
	SECTION:=Other Modules
	CATEGORY:=Local
	TITLE:=TI Memory Allocation module for communcations with the DSP
	URL:=http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/ce
	DEPENDS:=@TARGET_davinci +ti-xdc
	FILES:=$(PKG_BUILD_DIR)/kernel_binaries/dm6446/cmemk.ko
endef

define Package/ti-ce
	SECTION:=local
	CATEGORY:=Local
	TITLE:=TI Codec Engine
	DEPENDS:=@TARGET_davinci
endef

define Package/ti-ce/description
	TI Codec Engine Framework automates invocation/instantiaion of XDAIS-compliant algos such as DSP codecs
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	$(CP) ./src/* $(PKG_BUILD_DIR)/	
	# create a generic symlink to package so that other TI packages can find it
	rm -f $(BUILD_DIR)/$(PKG_NAME)
	ln -s $(PKG_BUILD_DIR) $(BUILD_DIR)/$(PKG_NAME)
endef

DVSDK_PATHS = \
	DMAI_INSTALL_DIR=$(BUILD_DIR)/ti-dmai2 \
	CE_INSTALL_DIR=$(BUILD_DIR)/ti-ce \
	DVSDK_INSTALL_DIR=$(BUILD_DIR)/ti-ce \
	CMEM_INSTALL_DIR=$(BUILD_DIR)/ti-ce/cetools \
	XDAIS_INSTALL_DIR=$(BUILD_DIR)/ti-ce/cetools \
	FC_INSTALL_DIR=$(BUILD_DIR)/ti-ce/cetools \
	XDC_INSTALL_DIR=$(BUILD_DIR)/ti-xdc \
	MVTOOL_DIR=$(TOOLCHAIN_DIR)/bin \
	MVTOOL_PREFIX=$(TARGET_CROSS) \
	LINUXKERNEL_INSTALL_DIR=$(LINUX_DIR) \

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CFLAGS="$(TARGET_CFLAGS) -I$(STAGING_DIR)/usr/include" \
		PLATFORM=dm6446 \
		LDFLAGS="-L$(STAGING_DIR)/usr/lib" \
		$(DVSDK_PATHS) \
		CROSS_COMPILE=$(TARGET_CROSS) \
		cmem
endef

define Build/Clean
	rm -f $(BUILD_DIR)/$(PKG_NAME)
endef

define Package/ti-ce/install
	/bin/true
endef


$(eval $(call BuildPackage,ti-ce))
$(eval $(call KernelPackage,ti-cmem))
