include $(TOPDIR)/rules.mk

PKG_NAME:=ti-dmai
PKG_REV:=645
PKG_VERSION:=r$(PKG_REV)
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=svn
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=ti-dmai-$(PKG_VERSION)
PKG_SOURCE_URL:=--username=anonymous --password= https://gforge.ti.com/svn/dmai/trunk/davinci_multimedia_application_interface/dmai
PKG_SOURCE:=$(PKG_SOURCE_SUBDIR).tar.bz2

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_SOURCE_SUBDIR)

include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/package.mk

define Package/ti-dmai
	CATEGORY:=Local
	URL:=https://gforge.ti.com/gf/project/dmai/
	TITLE:=TI DMAI
	DEPENDS:=@TARGET_davinci +ti-xdc +ti-cg +ti-dspbios +ti-dsplink +ti-ce +ti-codecs +alsa-lib
endef

define Package/ti-dmai/description
	TI Davinci Multimedia Application Interface (DMAI) is a thin utility layer on top of the OS (Linux or DSP/BIOS)
	and the Codec Engine (CE) to assist in writing portable applications for TI chips.
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	# create a generic symlink to package so that other TI packages can find it
	rm -f $(BUILD_DIR)/$(PKG_NAME)
	ln -s $(PKG_BUILD_DIR) $(BUILD_DIR)/$(PKG_NAME)
endef

EXTRA_CFLAGS += \
	-I$(STAGING_DIR)/usr/include \
	-Dxdc__RESTRICT__  \

EXTRA_LDFLAGS += \
	-L$(STAGING_DIR)/usr/lib \
	-Wl,-rpath-link=$(STAGING_DIR)/usr/lib \

DVSDK_PATHS = \
	DVSDK_INSTALL_DIR=$(BUILD_DIR)/ti-codecs \
	XDC_INSTALL_DIR=$(BUILD_DIR)/ti-xdc \
	DMAI_INSTALL_DIR=$(BUILD_DIR)/ti-dmai \
	BIOS_INSTALL_DIR=$(BUILD_DIR)/ti-dspbios \
	CODEGEN_INSTALL_DIR=$(BUILD_DIR)/ti-cg \
	CE_INSTALL_DIR=$(BUILD_DIR)/ti-ce \
	CMEM_INSTALL_DIR=$(BUILD_DIR)/ti-ce/cetools \
	FC_INSTALL_DIR=$(BUILD_DIR)/ti-ce/cetools \
	XDAIS_INSTALL_DIR=$(BUILD_DIR)/ti-ce/cetools \
	LINK_INSTALL_DIR=$(BUILD_DIR)/ti-dsplink/packages \
	LINUXKERNEL_INSTALL_DIR=$(LINUX_DIR) \
	CODEC_INSTALL_DIR=$(BUILD_DIR)/ti-codecs/dm6446_dvsdk_combos_2_05 \
	LINUXLIBS_INSTALL_DIR=$(STAGING_DIR)/usr \
	MVTOOL_PREFIX=$(TARGET_CROSS) \
	MVTOOL_DIR=$(TOOLCHAIN_DIR)/bin \

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CFLAGS="$(TARGET_CFLAGS) $(EXTRA_CFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS) $(EXTRA_LDFLAGS)" \
		$(DVSDK_PATHS) \
		CROSS_COMPILE=$(TOOLCHAIN_DIR)/bin/$(TARGET_CROSS) \
		VERBOSE=true \
		PLATFORM=dm6446_al
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CFLAGS="$(TARGET_CFLAGS) $(EXTRA_CFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS) $(EXTRA_LDFLAGS)" \
		$(DVSDK_PATHS) \
		CROSS_COMPILE=$(TOOLCHAIN_DIR)/bin/$(TARGET_CROSS) \
		VERBOSE=true \
		PLATFORM=dm6446_db
endef

define Build/Clean
	rm -f $(BUILD_DIR)/$(PKG_NAME)
endef

define Package/ti-dmai/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/packages/ti/sdo/dmai/apps/*/linux/*.x470uC $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/packages/ti/sdo/dmai/apps/*/bios/*.x64P $(1)/usr/bin
endef


$(eval $(call BuildPackage,ti-dmai))
