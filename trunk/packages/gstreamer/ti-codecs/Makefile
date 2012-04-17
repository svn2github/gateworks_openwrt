include $(TOPDIR)/rules.mk

PKG_NAME:=ti-codecs
PKG_VERSION:=2_00_00_22
PKG_MD5SUM:=2ce99015bb1ed1df0491403c5e8d99fb

PKG_SOURCE:=dm6446_codecs_setuplinux_$(PKG_VERSION).bin
PKG_BUILD_DIR:=$(BUILD_DIR)/dvsdk_$(PKG_VERSION)
PKG_SOURCE_URL:=http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/S1SDKLNX/DVSDK_2_00/exports/

include $(INCLUDE_DIR)/package.mk

define Package/ti-codecs
	CATEGORY:=Local
	URL:=http://www.ti.com/tool/dm644xcodecs
	TITLE:=TI Codec algorithms for audio and video
	DEPENDS:=@TARGET_davinci +ti-xdc +ti-cg +ti-dspbios +kmod-ti-dsplink +ti-ce
endef

define Package/ti-codecs/description
	TI Codec algorithm combos
endef

define Build/Prepare
	chmod +x $(DL_DIR)/$(PKG_SOURCE)
	$(DL_DIR)/$(PKG_SOURCE) --prefix $(BUILD_DIR) -DInstallMode Silent
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	ln -fs $(PKG_BUILD_DIR) $(BUILD_DIR)/$(PKG_NAME)
	$(call Build/Patch)
endef

DVSDK_PATHS = \
	DVSDK_INSTALL_DIR=$(PKG_BUILD_DIR) \
	DVEVM_INSTALL_DIR=$(PKG_BUILD_DIR) \
	DVSDK_PATH=$(BUILD_DIR)/ti-codecs \
	CODEC_INSTALL_DIR=$(BUILD_DIR)/ti-codecs/dm6446_dvsdk_combos_2_05 \
	XDC_INSTALL_DIR=$(BUILD_DIR)/ti-xdc \
	CE_INSTALL_DIR=$(BUILD_DIR)/ti-ce \
	DEMO_INSTALL_DIR=$(PKG_BUILD_DIR)/dvsdk_demos_2_00_00_07 \
	XDAIS_INSTALL_DIR=$(BUILD_DIR)/ti-ce/cetools \
	CMEM_INSTALL_DIR=$(BUILD_DIR)/ti-ce/cetools \
	FC_INSTALL_DIR=$(BUILD_DIR)/ti-ce/cetools \
	LINK_INSTALL_DIR=$(BUILD_DIR)/ti-dsplink \
	BIOS_INSTALL_DIR=$(BUILD_DIR)/ti-dspbios \
	USER_XDC_PATH=$(CE_INSTALL_DIR)/examples \
	CODEGEN_INSTALL_DIR=$(BUILD_DIR)/ti-cg \
	LINUXKERNEL_INSTALL_DIR=$(LINUX_DIR) \
	MVTOOL_DIR=$(TOOLCHAIN_DIR)/bin \
	MVTOOL_PREFIX=$(TARGET_CROSS) \
	EXEC_DIR=$(PKG_BUILD_DIR)/install/$(PLATFORM) \
	LINUXLIBS_INSTALL_DIR=$(TOOLCHAIN_DIR)/usr \
	KERNEL_DIR=$(LINUX_DIR) \
	TARGET_ROOT_DIR=$(PKG_BUILD_DIR) \

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		PLATFORM=dm6446 \
		$(DVSDK_PATHS) \
		CROSS_COMPILE=$(TARGET_CROSS) \
		COMPILER=$(TARGET_CC) \
		CROSS_COMPILER=MVL_5_0 \
		XDCARGS="prod" \
		codecs
endef

define Build/CompileFoo
	$(MAKE) -C $(PKG_BUILD_DIR) \
	$(TARGET_CONFIGURE_OPTS) PLATFORM=dm6446 CFLAGS="$(TARGET_CFLAGS) \
	-I$(STAGING_DIR)/usr/lib/libiconv-full/include \
	-I$(STAGING_DIR)/usr/lib/libintl-full/include \
	-I$(STAGING_DIR)/usr/include" \
	LDFLAGS="-L$(STAGING_DIR)/usr/lib/libintl-full/lib \
	-L$(STAGING_DIR)/usr/lib/libiconv-full/lib \
	-L$(STAGING_DIR)/usr/lib" \
	DVSDK_INSTALL_DIR=$(PKG_BUILD_DIR) \
	DVEVM_INSTALL_DIR=$(PKG_BUILD_DIR) \
	DEMO_INSTALL_DIR=$(PKG_BUILD_DIR)/dvsdk_demos_2_00_00_07 \
	CE_INSTALL_DIR=$(BUILD_DIR)/ti-ce \
	XDAIS_INSTALL_DIR=$(BUILD_DIR)/ti-ce/cetools \
	LINK_INSTALL_DIR=$(BUILD_DIR)/ti-dsplink \
	CMEM_INSTALL_DIR=$(BUILD_DIR)/ti-ce/cetools \
	CODEC_INSTALL_DIR=$(PKG_BUILD_DIR)/dm6446_dvsdk_combos_2_05 \
	XDC_INSTALL_DIR=$(BUILD_DIR)/ti-xdc \
	FC_INSTALL_DIR=$(BUILD_DIR)/ti-ce/cetools \
	BIOS_INSTALL_DIR=$(BUILD_DIR)/ti-dspbios \
	USER_XDC_PATH=$(CE_INSTALL_DIR)/examples \
	CODEGEN_INSTALL_DIR=$(BUILD_DIR)/ti-cg \
	LINUXKERNEL_INSTALL_DIR=$(LINUX_DIR) \
	MVTOOL_DIR=$(TOOLCHAIN_DIR)/bin \
	MVTOOL_PREFIX=$(TARGET_CROSS) \
	EXEC_DIR=$(PKG_BUILD_DIR)/install/$(PLATFORM) \
	LINUXLIBS_INSTALL_DIR=$(TOOLCHAIN_DIR)/usr \
	CROSS_COMPILE=$(TARGET_CROSS) \
	COMPILER=$(TARGET_CC) \
	ARCHIVER=$(AR) \
	KERNEL_DIR=$(LINUX_DIR) \
	TARGET_ROOT_DIR=$(PKG_BUILD_DIR) \
	CROSS_COMPILER=MVL_5_0 \
	OBJDUMP=$(TARGET_CROSS)objdump \
	XDCARGS="prod" \
	codecs
endef

define Build/Clean
	rm -f $(BUILD_DIR)/$(PKG_NAME)
endef

define Package/ti-codecs/install
	$(INSTALL_DIR) $(1)/etc/ti
	$(CP) $(PKG_BUILD_DIR)/dm6446_dvsdk_combos_2_05/packages/ti/sdo/servers/decode/decodeCombo.x64P $(1)/etc/ti/
	$(CP) $(PKG_BUILD_DIR)/dm6446_dvsdk_combos_2_05/packages/ti/sdo/servers/loopback/loopbackCombo.x64P $(1)/etc/ti/
	$(CP) $(PKG_BUILD_DIR)/dm6446_dvsdk_combos_2_05/packages/ti/sdo/servers/encode/encodeCombo.x64P $(1)/etc/ti/
endef

$(eval $(call BuildPackage,ti-codecs))
